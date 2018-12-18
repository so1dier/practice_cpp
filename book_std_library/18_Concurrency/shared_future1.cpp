#include <future>
#include <thread>
#include <exception>
#include <stdexcept>
#include <iostream>

int queryNumber()
{
    //read number
    std::cout << "read number: ";
    int num;
    std::cin >> num;

    //throw exception if none
    if (!std::cin)
    {
        throw std::runtime_error("no number read");
    }

    return num;
}

void doSomething(char c, std::shared_future<int> f)
{

    try
    {
        int num = f.get();
        for (int i=0; i<num; ++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout.put(c).flush();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Excpetion in thread " << std::this_thread::get_id() << ": " << e.what() << std::endl;

    }
}

int main()
{
    try
    {
        std::shared_future<int> f = std::async(queryNumber);
        auto f1 = std::async(std::launch::async, doSomething, '.', f);
        auto f2 = std::async(std::launch::async, doSomething, '+', f);
        auto f3 = std::async(std::launch::async, doSomething, '*', f);
        f1.get();
        f2.get();
        f3.get();
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception" << e.what() << std::endl;
    }
    std::cout << "Done";
}
