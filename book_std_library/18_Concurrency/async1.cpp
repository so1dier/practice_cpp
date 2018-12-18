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
        std::this_thread::sleep_for(std::chrono::milliseconds(id(dre)));
        std::cout.put(c).flush();
    }

    return c;

}

int func1()
{
    return doSomething('.');
}

int func2()
{
    return doSomething('+');
}

int main()
{
    std::cout << "starting func1() in background and func2() in foreground:" << std::endl;

    //std::future<int> result1(std::async(func1));
    std::future<int> result1(std::async(std::launch::deferred,func1));
    int result2 = func2();

    int result = result1.get() + result2;

    std::cout << "result of both = " << result << std::endl;
}
