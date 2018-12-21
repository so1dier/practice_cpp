#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>
#include <iostream>
#include <queue>

std::queue<int> queue_in;
std::mutex mtx_queue_in;
std::condition_variable cv_queue_in;

std::queue<int> queue_out;
std::mutex mtx_queue_out;
std::condition_variable cv_queue_out;

void provider (int val)
{
    for (int i=0; i<6; ++i)
    {
        std::lock_guard<std::mutex> lg(mtx_queue_out);
        queue_out.push(val+i);
    }
    cv_queue_out.notify_one();

    //std::this_thread::sleep_for(std::chrono::milliseconds(val));
}

void consumer(int num)
{
    while(true)
    {
        int val;
        {
            std::unique_lock<std::mutex> ul(mtx_queue_out);
            if(cv_queue_out.wait_for(ul, std::chrono::milliseconds(500), []{ return !queue_out.empty();}))
            {
                val = queue_out.front();
                queue_out.pop();
            }
            else
            {

            }
        }
        std::cout << "consumer " << num << ": " << val << std::endl;
    }
}

int main()
{
    auto p1 = std::async(std::launch::async, provider,100);
    auto p2 = std::async(std::launch::async, provider,300);
    auto p3 = std::async(std::launch::async, provider,500);

    auto c1 = std::async(std::launch::async, consumer,1);
    auto c2 = std::async(std::launch::async, consumer,2);
}
