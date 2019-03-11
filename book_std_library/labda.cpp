#include <iostream>
#include <string>


//Lambda rules:
// [...] (...) mutable(opt) throw (opt) -> ret_type (opt)
// [=] - the outer scope passed to lambda by value. read but not modify
// [&] - the outer scope passed to lambda by reference. You have write access to all data that was valid when the lambda was defined.
//
// also limit access is possible:
// [x, &y] same as [=, &y] - = means all other objects by value


int main()
{

    auto l = [](const std::string& str) { std::cout << "Passed str is: " << str << std::endl; };

    l("Hello Lambda!");


    auto l2 = []() -> double { return 42; };

    std::cout << l2() << std::endl;

}
