#include <thread>
#include <future>
#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>
#include <functional>
#include <utility>

void DoSome(std::promise<std::string>& p)
{
    try
    {
        std::cout << "read char ('x' for exception): ";
        char c = std::cin.get();
        if (c=='x')
        {
            throw std::runtime_error(std::string("char ")+c+"read");
        }
        std::string s = std::string("char ") +c + " processed";
        p.set_value(std::move(s));
    }
    catch(...)
    {
        p.set_exception(std::current_exception());
    }
}

int main()
{
    try
    {
        //start thrad using a promise to store the outcome
        std::promise<std::string> p;
        std::thread t(DoSome, std::ref(p));
        t.detach();

        //create a future to process the outcome
        std::future<std::string> f(p.get_future());

        //process the outcome
        std::cout << "result: " << f.get() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Excpetion: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Exception " << std::endl;
    }
}
