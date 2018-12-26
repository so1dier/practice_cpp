#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "Socket.h"

std::queue<std::string> queue_in;
std::mutex mtx_queue_in;
std::condition_variable cv_queue_in;

std::queue<std::string> queue_out;
std::mutex mtx_queue_out;
std::condition_variable cv_queue_out;

std::mutex mtx_write;


void write(const std::string message)
{
    {
        std::unique_lock<std::mutex> l(mtx_write);
        std::cout << message << std::endl;
    }
}

void provider_out(int val)
{
    for (int i=0; i<6; ++i)
    {
        std::string request("request" + std::to_string(val+i));
        std::lock_guard<std::mutex> lg(mtx_queue_out);
        queue_out.push(request);
    }
    cv_queue_out.notify_one();

    //std::this_thread::sleep_for(std::chrono::milliseconds(val));
}

void provider_in(std::string response)
{
    {
        std::lock_guard<std::mutex> lg(mtx_queue_in);
        queue_in.push(response);
    }
    cv_queue_in.notify_one();

    //std::this_thread::sleep_for(std::chrono::milliseconds(val));
}

void consumer_out(int num, int port)
{
    const std::string ip("127.0.0.1"); 
    const int timeout = 10000;
    Socket s(ip, port, timeout);
    while(true)
    {
        int val;
        std::string request;
        {
            std::unique_lock<std::mutex> ul(mtx_queue_out);
            //if(cv_queue_out.wait_for(ul, std::chrono::milliseconds(500), []{ return !queue_out.empty();}))
            cv_queue_out.wait(ul, [&]{ return !queue_out.empty() && s.IsConnected();});
            {
                std::string request = queue_out.front();
                queue_out.pop();
                std::string response;
                s.SendAndReceive(request, response);
                provider_in(response);
            }
        }
        write(std::string(__FUNCTION__) + " " + std::to_string(num) + ": " + request);
    }
}


void consumer_in(int num)
{
    while(true)
    {
        std::string response;
        {
            std::unique_lock<std::mutex> ul(mtx_queue_in);
            cv_queue_in.wait(ul, []{ return !queue_in.empty();});
            {
                response = queue_in.front();
                queue_in.pop();
            }
        }
        write(std::string(__FUNCTION__) + " " + std::to_string(num) + ": " + response);
    }
}

int main(int argc, char* argv[])
{
    std::vector<int> ports;
    if (argc > 0)
    {
        for (int i=1; i!=argc; ++i)
        {
            std::cout << "argv i= " << i << " = " << argv[i] << std::endl;
            std::string ip_string(argv[i]);
            ports.push_back(std::stoi(ip_string));
        }
    }

    auto p1 = std::async(std::launch::async, provider_out,100);
    auto p2 = std::async(std::launch::async, provider_out,200);
    auto p3 = std::async(std::launch::async, provider_out,300);
    auto p4 = std::async(std::launch::async, provider_out,400);
    auto p5 = std::async(std::launch::async, provider_out,500);

    //INSTEAD of separate variable, use a vector of futures
    //auto c1 = std::async(std::launch::async, consumer_out,1, 8888);
    //auto c2 = std::async(std::launch::async, consumer_out,2, 8889);
    std::vector<std::future<void>> consumer_outs;
    for (int i=0; i!=ports.size(); ++i)
    {
        consumer_outs.push_back(std::async(std::launch::async, consumer_out,1,ports[i]));
    }

    auto ci1 = std::async(std::launch::async, consumer_in, 1);
    auto ci2 = std::async(std::launch::async, consumer_in, 2);

    //Doesn't matter as get() is called on destruction of each future
    //for (int i=0; i!=ports.size(); ++i)
    //{
    //    consumer_outs[i].get();
    //}
}
