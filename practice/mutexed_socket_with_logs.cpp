#include <mutex>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/time.h>


#define LOG(message) log(std::string(__FUNCTION__) + "(): " + message)

std::mutex mtx_messages_out;
std::mutex mtx_messages_in;
std::mutex mtx_print_out;
std::mutex mtx_reply;
std::vector<std::string> messages_out;
std::vector<std::string> messages_in;

void log(const std::string message)
{
    {
        time_t rawtime;
        time (&rawtime);

        struct tm * timeinfo;
        timeinfo = localtime (&rawtime);
         
        char buffer[80];
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S",timeinfo);

        struct timeval time_ms;
        gettimeofday(&time_ms, NULL);
        //uint64_t millis = (time_ms.tv_sec * (uint64_t)1000) + (time_ms.tv_usec / 1000);
        uint64_t millis = (time_ms.tv_usec / 1000);

        mtx_print_out.lock();
        std::cout << buffer << "." << millis << " " << message << std::endl;
        mtx_print_out.unlock();
     }
}

void SocketThread(const std::string ip, int port, int timeout)
{
    std::stringstream ss_id;
    ss_id << std::this_thread::get_id();
    LOG("Thread started id=" + ss_id.str() + " ip: " + ip + " port " + std::to_string(port) + " timeout " + std::to_string(timeout));

    int max_count = 2;
    int counter = 0;

    auto now = std::chrono::system_clock::now();
    auto ttl = now + std::chrono::milliseconds(timeout);

    while(ttl > std::chrono::system_clock::now())
    {
        mtx_messages_out.lock();
        auto local_messages_out = std::move(messages_out);
        //messages_out = {};
        mtx_messages_out.unlock();
        if(local_messages_out.empty() == false)
        {
            for(auto message : local_messages_out)
            {
                LOG("sending message: " + message);
                mtx_messages_in.lock();
                messages_in.push_back("OK..." + message);
                mtx_messages_in.unlock();
            }
        }
    }

    LOG("Thread terminated " + ss_id.str());
}

void SendMessage(std::string message)
{
    mtx_messages_out.lock();
    LOG("Adding message into the vector: ");
    messages_out.push_back(message);
    mtx_messages_out.unlock();
}

void ReceiveReply(std::string& reply, int timeout)
{
    bool received = false;

    auto ttl = std::chrono::system_clock::now() + std::chrono::milliseconds(timeout);
    while(ttl > std::chrono::system_clock::now() && received == false)
    {
        mtx_messages_in.lock();
        auto local_messages_in = std::move(messages_in);
        //messages_in = {};
        mtx_messages_in.unlock();

        if(local_messages_in.empty() == false)
        {
            LOG("local_messages_in not empty");
            for (auto message : local_messages_in)
            {
                mtx_reply.lock();
                reply = message;
                mtx_reply.unlock();
                LOG("Received a reply : " + reply);
                received = true;
            }
        }
    }

    LOG("ttl expired or message received");
}

int main()
{
    int timeout = 10000;
    std::thread t1(SocketThread,"127.0.0.1", 8888, timeout);
    //std::thread t2(SocketThread,"127.0.0.1", 8889, timeout);

    auto now = std::chrono::system_clock::now();
    auto ttl = now + std::chrono::milliseconds(timeout);

    int counter=0;
    while(ttl > std::chrono::system_clock::now())
    {
        std::string request("request" + std::to_string(counter));
        SendMessage(request);
        std::string response;
        ReceiveReply(response, timeout);
        mtx_reply.lock();
        LOG("This is a reply in main: " + response);
        mtx_reply.unlock();
        ++counter;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    t1.join();
    //t2.join();
}
