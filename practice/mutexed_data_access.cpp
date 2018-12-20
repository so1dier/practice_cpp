#include <mutex>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/time.h>

#define LOG(message) log(std::string(__FUNCTION__) + "(): " + message)

std::mutex mtx_print_out;
std::mutex mtx_reply;

class Data
{
public:
    std::vector<std::string>& GetMessagesIn();
    std::vector<std::string>& GetMessagesOut();
    void SetMessagesIn(const std::string message);
    void SetMessagesOut(const std::string message);
    bool GetStatus();
    void SetStatus(bool status);
private:
    std::mutex mtx_messages_out;
    std::mutex mtx_messages_in;
    std::vector<std::string> messages_out;
    std::vector<std::string> messages_in;
    bool m_status = false;
    std::mutex mtx_status;
};

std::vector<std::string>& Data::GetMessagesIn()
{
    std::unique_lock<std::mutex> l(mtx_messages_in);
    return messages_in;
}

std::vector<std::string>& Data::GetMessagesOut()
{
    std::unique_lock<std::mutex> l(mtx_messages_out);
    return messages_out;
}

void Data::SetMessagesIn(const std::string message)
{
    std::unique_lock<std::mutex> l(mtx_messages_in);
    messages_in.push_back(message);
}

void Data::SetMessagesOut(const std::string message)
{
    std::unique_lock<std::mutex> l(mtx_messages_out);
    messages_out.push_back(message);
}

bool Data::GetStatus()
{
    std::unique_lock<std::mutex> l(mtx_status);
    m_status = !m_status;
    return m_status;
}

void Data::SetStatus(bool status)
{
    std::unique_lock<std::mutex> l(mtx_status);
    m_status = status;
}

Data data;


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
        auto local_messages_out = std::move(data.GetMessagesOut());

        if (local_messages_out.empty() == false)
        {
            for(auto message : local_messages_out)
            {
                LOG("sending message: " + message);
                data.SetMessagesIn("OK..." + message);
            }
        }
    }

    LOG("Thread terminated " + ss_id.str());
}

void SendMessage(std::string message)
{
    LOG("Adding message into the vector: ");
    data.SetMessagesOut(message);
}

void ReceiveReply(std::string& reply, int timeout)
{
    bool received = false;

    auto ttl = std::chrono::system_clock::now() + std::chrono::milliseconds(timeout);
    while(ttl > std::chrono::system_clock::now() && received == false)
    {
        auto local_messages_in = std::move(data.GetMessagesIn());

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
        auto local_status_1 = data.GetStatus();
        if (local_status_1)
        {
            SendMessage(request);
            std::string response;
            ReceiveReply(response, timeout);
            mtx_reply.lock();
            LOG("This is a reply in main: " + response);
            mtx_reply.unlock();
            ++counter;
        }
        else
        {
            LOG("Status1 is bad, looking for next one...");
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    t1.join();
    //t2.join();
}
