#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>
#include <iostream>
#include <queue>
#include <string>

std::queue<int> queue_in;
std::mutex mtx_queue_in;
std::condition_variable cv_queue_in;

std::queue<int> queue_out;
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
        std::lock_guard<std::mutex> lg(mtx_queue_out);
        queue_out.push(val+i);
    }
    cv_queue_out.notify_one();

    //std::this_thread::sleep_for(std::chrono::milliseconds(val));
}

void provider_in(int val)
{
    {
        std::lock_guard<std::mutex> lg(mtx_queue_in);
        queue_in.push(val);
    }
    cv_queue_in.notify_one();

    //std::this_thread::sleep_for(std::chrono::milliseconds(val));
}

void consumer_out(int num)
{
    while(true)
    {
        int val;
        {
            std::unique_lock<std::mutex> ul(mtx_queue_out);
            //if(cv_queue_out.wait_for(ul, std::chrono::milliseconds(500), []{ return !queue_out.empty();}))
            cv_queue_out.wait(ul, []{ return !queue_out.empty();});
            {
                val = queue_out.front();
                queue_out.pop();
                provider_in(val);
            }
        }
        //std::cout <<__FUNCTION__ << " " << num << ": " << val << std::endl;
        write(std::string(__FUNCTION__) + " " + std::to_string(num) + ": " + std::to_string(val));
    }
}


void consumer_in(int num)
{
    while(true)
    {
        int val;
        {
            std::unique_lock<std::mutex> ul(mtx_queue_in);
            cv_queue_in.wait(ul, []{ return !queue_in.empty();});
            {
                val = queue_in.front();
                queue_in.pop();
            }
        }
        //std::cout << __FUNCTION__ << " " << num << ": " << val << std::endl;
        //write(__FUNCTION__ + " " + num + ": " + val);
        write(std::string(__FUNCTION__) + " " + std::to_string(num) + ": " + std::to_string(val));
    }
}

int main()
{
    auto p1 = std::async(std::launch::async, provider_out,100);
    auto p2 = std::async(std::launch::async, provider_out,300);
    auto p3 = std::async(std::launch::async, provider_out,500);

    auto c1 = std::async(std::launch::async, consumer_out,1);
    auto c2 = std::async(std::launch::async, consumer_out,2);

    auto ci1 = std::async(std::launch::async, consumer_in, 1);
    auto ci2 = std::async(std::launch::async, consumer_in, 2);
}
