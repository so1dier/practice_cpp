#include <tuple>
#include <iostream>
#include <complex>
#include <string>
#include <functional>

int main()
{
    std::tuple<std::string, int, int, std::complex<double>> t;

    std::tuple<int, float, std::string> t1(41, 6.3, "nico");

    std::cout << std::get<0>(t1) << " ";
    std::cout << std::get<1>(t1) << " ";
    std::cout << std::get<2>(t1) << " " << std::endl;

    auto t2 = std::make_tuple(7, 13, "Dan");


    std::get<1>(t1) = std::get<1>(t2);

    auto t3 = std::make_tuple(7, "wow", 12.12, 'G');
    auto t4 = std::make_tuple(1,"no", 4.4, 'D');

    t3.swap(t4);


    //modify a value in tuple
    std::string s1;
    auto t5 = std::make_tuple(s1);
    std::get<0>(t5) = "my value"; //modifies x but not s

    std::string s2;
    auto t6 = std::make_tuple(std::ref(s2));
    s2 = "changing both values";
    std::cout << "s2=" << s2 << ", t5=" << std::get<0>(t6) << std::endl;

    
}
