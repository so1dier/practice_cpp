#include <thread>
#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include <chrono>


std::mutex m;
std::condition_variable cv;
std::string data;
bool ready=false;
bool processed=false;


void WorkerThreadWait()
{
    //wait until main() sends data
    std::unique_lock<std::mutex> lk(m);
    //WAIT
    cv.wait(lk, []{return ready;});

    //after the wait, we own the lock.
    std::cout << __FUNCTION__ << " thread is processing data" << std::endl;
    data += " after processing";

    //send data back to main()
    processed = true;
    std::cout << __FUNCTION__ << " thread signals data processing completed" << std::endl;

    //Manual unlocking is done before notifying, to avoid waking up
    //the waiting rhead onlu to block again 
    lk.unlock();
    cv.notify_one();
}

void WorkerThreadWaitFor()
{
    //wait until main() sends data
    std::unique_lock<std::mutex> lk(m);
    //WAIT_FOR
    
    if(cv.wait_for(lk, 
                   std::chrono::seconds(1),
                   []{return ready;}))
    
    {
        std::cout << __FUNCTION__ << " thread finished waiting" << std::endl;
    }
    else
    {
        std::cout << __FUNCTION__ << " thread timed out" << std::endl;
    }

    //after the wait, we own the lock.
    std::cout << __FUNCTION__ << " thread is processing data" << std::endl;
    data += " after processing";

    //send data back to main()
    processed = true;
    std::cout << __FUNCTION__ << "thread signals data processing completed" << std::endl;

    //Manual unlocking is done before notifying, to avoid waking up
    //the waiting rhead onlu to block again 
    lk.unlock();
    cv.notify_one();
}

void WorkerThreadWaitUntil()
{
    //wait until main() sends data
    std::unique_lock<std::mutex> lk(m);
    //WAIT_UNTIL
     
    if(cv.wait_until(lk, 
                     std::chrono::system_clock::now() + std::chrono::seconds(1),
                     []{return ready;}))
    {
        std::cout << __FUNCTION__ << " thread finished waiting" << std::endl;
    }
    else
    {
        std::cout << __FUNCTION__ << " thread timed out" << std::endl;
    }

    //after the wait, we own the lock.
    std::cout << __FUNCTION__ << " thread is processing data" << std::endl;
    data += " adter processing";

    //send data back to main()
    processed = true;
    std::cout << __FUNCTION__ << " thread signals data processing completed" << std::endl;

    //Manual unlocking is done before notifying, to avoid waking up
    //the waiting rhead onlu to block again 
    lk.unlock();
    cv.notify_one();
}

int main()
{
    //std::thread worker_wait(WorkerThreadWait);
    std::thread worker_wait_for(WorkerThreadWaitFor);
    std::thread worker_wait_until(WorkerThreadWaitUntil);

    while (1)
    {

    data = "Example data";
    //send data to worker thread
    {
        std::lock_guard<std::mutex> lk(m);
        //ready = true;
        ready = false;
        std::cout << "main() signals data ready for processing" << std::endl;
    }
    cv.notify_one();

    //wait for worker
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, []{return processed;});
    }

    std::cout << "Back in main(), data = " << data << std::endl;
    }

    //worker_wait.join();
    worker_wait_for.join();
    worker_wait_until.join();

}

