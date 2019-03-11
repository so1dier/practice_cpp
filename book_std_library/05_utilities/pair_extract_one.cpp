#include <utility>
#include <tuple>
#include <iostream>

int main()
{
    std::pair<char, char> c = std::make_pair('D', 'G');

    char a;
    std::tie(std::ignore, a) = c;

    std::cout << "a is " << a << std::endl;

}

