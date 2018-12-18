#include <future>
#include <thread>
#include <chrono>
#include <random>
#include <iostream>
#include <exception>

int doSomething(char c)
{
    std::default_random_engine dre(c);
    std::uniform_int_distribution<int> id(10,1000);

    for (int i=0; i<10; ++i)
    {
//        std::this_thread::sleep_for(std::chrono::milliseconds(id(dre)));
        std::cout.put(c).flush();
    }

    return c;

}

int main()
{
    std::cout << "starting 2 operations async" << std::endl;

    auto f1 = std::async([]{doSomething('.');});
    auto f2 = std::async([]{doSomething('+');});

    //if at least one of the background tasks is running
    if (f1.wait_for(std::chrono::seconds(0)) != std::future_status::deferred ||
        f2.wait_for(std::chrono::seconds(0)) != std::future_status::deferred)
    {
        //poll until at least one of the loops finished
        while (f1.wait_for(std::chrono::seconds(0)) != std::future_status::ready &&
               f2.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
        {
            std::this_thread::yield();
        }
    }
    std::cout.put('\n').flush();

    //wait for all loops to finish and process any exceptions
    try
    {
        f1.get();
        f2.get();
    }
    catch(const std::exception& e)
    {
        std::cout << "\nException: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "\nCrazy Exception: "<< std::endl;
    }
    std::cout << "Done" << std::endl;
}
