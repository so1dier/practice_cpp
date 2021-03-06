#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <condition_variable>

bool terminateLoggerThread = false;
std::mutex logMessagesMutex;
std::condition_variable logMessagesCv;
std::vector<std::string> logMessages;

std::thread loggerThread { 
    []() {
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
};

bool logMessage(std::string message)
{
    if(!loggerThread.joinable())
        return false;
    
    std::unique_lock<std::mutex> lock(logMessagesMutex);
    logMessages.push_back(std::move(message));
    lock.unlock();
    logMessagesCv.notify_one();
    return true;
}

int main()
{    
    logMessage("some text");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    logMessage("some more text");
    terminateLoggerThread = true;
    logMessagesCv.notify_one();
    loggerThread.join(); // can't log after this
}
