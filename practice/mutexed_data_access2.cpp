#include <mutex>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <condition_variable>

bool terminateLoggerThread = false;
std::mutex logMessagesMutex;
std::condition_variable logMessagesCv;
std::vector<std::string> logMessages;

void LoggerThread() { 
    //[]() {
        while(!terminateLoggerThread)
        {
            std::unique_lock<std::mutex> lock(logMessagesMutex);
            logMessagesCv.wait(lock, []{return logMessages.empty() == false || terminateLoggerThread;});
        
            auto messages = std::move(logMessages);
            lock.unlock(); // unblock other threads before logging
            //start logging
            for (auto& message : messages)
                std::cout << message << std::endl;
        }
    }

bool logMessage(std::string message)
{
//    if(!loggerThread.joinable())
//        return false;
    
    std::unique_lock<std::mutex> lock(logMessagesMutex);
    logMessages.push_back(std::move(message));
    lock.unlock();
    logMessagesCv.notify_one();
    return true;
}

std::mutex mtx_messages_out;
std::mutex mtx_messages_in;
//std::mutex mtx_print_out;
bool con1=true;
bool con2=true;
bool con3=true;
std::vector<std::string> messages_out;
std::vector<std::string> messages_in;

//void log(const std::string message)
//{
//    {
//        //std::lock_guard<std::mutex> lock(mtx_print_out);
//        mtx_print_out.lock();
//        std::cout << message << std::endl;
//        mtx_print_out.unlock();
//    }
//}

void Connection(const std::string ip, int port, int timeout)
{
    //std::this_thread::sleep_for(std::chrono::milliseconds(200));
    auto id = std::this_thread::get_id();
    logMessage("Thread started " + id);

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
            logMessage("Message_out is not empty");
            for(auto message : local_messages_out)
            {
                logMessage("sending message: " + message);
                messages_in.push_back("OK..." + message);
                mtx_messages_in.unlock();
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    logMessage("Thread terminated ");
}

void SendMessage(std::string message)
{
    mtx_messages_out.lock();
    logMessage("Adding message into the vector: ");
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
            {
                std::lock_guard<std::mutex> lock(mtx_print_out);
                logMessage("local_messages_in not empty");
            }
            for (auto message : local_messages_in)
            {
                reply = message;
                {
                    std::lock_guard<std::mutex> lock(mtx_print_out);
                    logMessage("Received a reply : ");
                }
                received = true;
            }
        }
    }

    logMessage("ttl expired or message received");
}

int main()
{
    int timeout = 5000;
    std::thread loggerThread(LoggerThread);
    std::thread t1(Connection,"127.0.0.1", 8888, timeout);
    //std::thread t2(Connection,"127.0.0.1", 8889, timeout);

    {
        std::string request("request1");
        SendMessage(request);
        std::string response;
        ReceiveReply(response, timeout);
        logMessage("This is a reply in main: " + response);
    }
    {
        std::string request("request2");
        SendMessage(request);
        std::string response;
        ReceiveReply(response, timeout);
        logMessage( "This is a reply in main: " + response);
    }

    t1.join();
    //t2.join();
}
