#include <memory>
#include <iostream>
#include <string>

int main()
{
    try
    {
        std::shared_ptr<std::string> sp(new std::string("hi"));
        std::weak_ptr<std::string> wp = sp;

        sp.reset();
        std::cout << "wp.use_count(): " << wp.use_count() << std::endl;
        std::cout << "wp.expired(): " << std::boolalpha << wp.expired() << std::endl;
        std::shared_ptr<std::string> p(wp);
    }
    catch (const std::exception& e)
    {
        std::cerr << "exception: " << e.what() << std::endl;
    }
}
