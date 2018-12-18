#include <thread>
#include <iostream>
#include <mutex>
#include <thread>

//Mutex is a lockable object that is designed to signal when critical sections of code need exclusive access,
//preventing other threads with the same protection from executing concurrently and access the same memory locations

std::mutex mtx;


void doAsync(const int repeat, const char ch)
{
    mtx.lock();
    for(int i=0; i!=repeat; ++i)
    {
        std::cout << ch;
    }
    std::cout << std::endl;
    mtx.unlock();
}

int main()
{
    std::thread t1(doAsync, 50, '*');
    std::thread t2(doAsync, 50, '$');


    t1.join();
    t2.join();

}
