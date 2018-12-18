#include <mutex>
#include <thread>
#include <iostream>
#include <string>
#include <vector>

std::mutex mtx_messages_out;
std::mutex mtx_messages_in;
bool con1=true;
bool con2=true;
bool con3=true;
std::vector<std::string> messages_out;
std::vector<std::string> messages_in;

void Connection(const std::string ip, int port, int timeout)
{
    //std::this_thread::sleep_for(std::chrono::milliseconds(200));
    auto id = std::this_thread::get_id();
    std::cout << "Thread started " << id << std::endl; 

    int max_count = 2;
    int counter = 0;
    while(++counter < max_count)
    {
        mtx_messages_out.lock();
        auto local_messages_out = std::move(messages_out);
        messages_out = {};
        mtx_messages_out.unlock();
        if(local_messages_out.empty() == false)
        {
            std::cout << "Message_out is not empty" << std::endl;
            for(auto message : local_messages_out)
            {
                mtx_messages_in.lock();
                std::cout << "sending message: " << message << std::endl;
                messages_in.push_back("OK..." + message);
                mtx_messages_in.unlock();
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Thread terminated " << id << std::endl; 
}

void SendMessage(std::string message)
{
    mtx_messages_out.lock();
    std::cout << "Adding message into the vector: " << message << std::endl;
    messages_out.push_back(message);
    mtx_messages_out.unlock();
}

void ReceiveReply(std::string& reply, int timeout)
{
    std::cout << __FUNCTION__ << std::endl;

    bool received = false;

    auto ttl = std::chrono::system_clock::now() + std::chrono::milliseconds(timeout);
    while(ttl > std::chrono::system_clock::now() && received == false)
    {
        mtx_messages_in.lock();
        auto local_messages_in = std::move(messages_in);
        messages_in = {};
        mtx_messages_in.unlock();

        if(local_messages_in.empty() == false)
        {
            std::cout << "local_messages_in not empty" << std::endl;
            for (auto message : local_messages_in)
            {
                reply = message;
                std::cout << "Received a reply : " << reply << std::endl;
                received = true;
            }
        }
    }
    std::cout << "ttl expired or message received" << std::endl;

}


int main()
{
    int timeout = 5000;
    std::thread t1(Connection,"127.0.0.1", 8888, timeout);

    {
    std::string request("request1");
    SendMessage(request);
    std::string response;
    ReceiveReply(response, timeout);
    std::cout << "This is a reply in main: " << response << std::endl;
    }

    t1.join();
}
