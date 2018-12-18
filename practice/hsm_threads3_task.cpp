#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <chrono>

#include "Socket.h"

bool terminateLoggerThread = false;
std::mutex logMessagesMutex;
std::condition_variable logMessagesCv;
std::vector<std::string> logMessages;


//std::thread loggerThread 
//{ 
//    []()
void SocketThread(std::string ip, int port, int timeout)
    {
        //std::string ip = "127.0.0.1";
        //int port = 8888;
        //int timeout = 10000;
        Socket socket(ip, port, timeout);

        while(!terminateLoggerThread)
        {
            std::unique_lock<std::mutex> lock(logMessagesMutex);
            if (logMessagesCv.wait_for(lock, 
                                       std::chrono::milliseconds(500), 
                                       []{return logMessages.empty() == false || terminateLoggerThread;}))
            {
                auto messages = std::move(logMessages);
                lock.unlock(); // unblock other threads before logging
                //start logging
                for (auto& request: messages)
                {
                    std::string response;
                    socket.SendAndReceive(request, response);
                    std::cout << "Response: " << response << std::endl;
                }
            }
            else
            {
                std::cout << "Timed out, sending ping..." << std::endl;
                std::string request("...ping...");
                std::string response;
                socket.SendAndReceive(request, response);
                std::cout << "Response: " << response << std::endl;
            }
        }
    }
//};

//std::thread loggerThread(SocketThread);

bool logMessage(std::string message)
{
    //if(!loggerThread.joinable())
    //    return false;
    
    std::unique_lock<std::mutex> lock(logMessagesMutex);
    logMessages.push_back(std::move(message));
    lock.unlock();
    logMessagesCv.notify_one();
    return true;
}

int main()
{    
    std::string ip = "127.0.0.1";
    int port = 8888;
    int timeout = 10000;
    Socket socket(ip, port, timeout);

    //std::string request("request1");
    //std::string response;
    //socket.SendAndReceive(request, response);


    std::thread loggerThread(SocketThread, ip, port, timeout);


    logMessage("request1");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    logMessage("request2");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    logMessage("request3");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    logMessage("request4");
    terminateLoggerThread = true;
    logMessagesCv.notify_one();
    loggerThread.join(); // can't log after this
}
