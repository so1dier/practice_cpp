#include <iostream>

template <typename T1, typename T2>
auto add(T1 x, T2 y) -> decltype(x+y);

int main()
{
    std::cout << add(1+2) << std::endl;
    std::cout << add(1.5+2.7) << std::endl;
}
