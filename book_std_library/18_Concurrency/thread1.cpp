#include <thread>
#include <chrono>
#include <random>
#include <iostream>
#include <exception>

void doSomething(int num, char c)
{
    try
    {
        std::default_random_engine dre(42*c);
        std::uniform_int_distribution<int> id(10,1000);

        for (int i=0; i<num; ++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(id(dre)));
            std::cout.put(c).flush();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Thread exception (thread " << std::this_thread::get_id() << "):" << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Thread exception (thread " << std::this_thread::get_id() << std::endl;
    }
}

int main()
{
    try
    {
        std::thread t1(doSomething,5,'.');
        std::cout << "- started fg thread " << t1.get_id() << std::endl;

        for (int i=0; i<5; ++i)
        {
            std::thread t(doSomething,10,'a'+i);
            std::cout << "Detach started" << t.get_id() << std::endl;
            t.detach();
        }
        std::cin.get();
        std::cout << "-join fg thread" << t1.get_id() << std::endl;
        t1.join();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Thread exception (thread " << e.what() << std::endl;
    }
}
