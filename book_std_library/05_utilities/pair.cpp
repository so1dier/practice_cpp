#include <utility>
#include <string>
#include <iostream>

void IsSame(std::pair<std::string, int> p1,
            std::pair<std::string, int> p2)
{
    if (p1 == p2)
    {
        std::cout << "Pairs are same" << std::endl;
    }
    else
    {
        std::cout << "Pairs are different" << std::endl;
    }
}

void IsMore(std::pair<std::string, int> p1,
            std::pair<std::string, int> p2)
{
    if (p1 > p2)
    {
        std::cout << "Pair is more" << std::endl;
    }
    else
    {
        std::cout << "Pairs is less" << std::endl;
    }
}

void PrintByFirst(std::string name, 
                  const std::pair<std::string, int> p)
{
    std::cout << name << " Pair first is: " << p.first
              << " second is: " << p.second << std::endl;
}

void PrintByGet(std::string name, 
                const std::pair<std::string, int> p)
{
    std::cout << name << " Pair get<0> is: " << std::get<0>(p)
              << " get<1> is: " << std::get<1>(p) << std::endl;
}

template<typename T1, typename T2>
std::ostream& operator<< (std::ostream& out,
                          const std::pair<T1, T2>& p)
{
    return out << "[" << p.first << "," << p.second << "]";
}

//template<typename T1, typename T2>
//bool operator==(const std::pair<T1, T2>& p1, const std::pair<T1, T2>& p2)
//{
//    return p1.first == p2.first && p1.second == p2.second;
//}
//
//template<typename T1, typename T2>
//bool operator<(const std::pair<T1, T2>& p1, const std::pair<T1, T2>& p2)
//{
//    return p1.first < p2.first || p1.second < p2.second;
//}
int main()
{
    std::pair<std::string, int> my_pair1{ "Daniel", 7};
    std::pair<std::string, int> my_pair2("Daniel", 13);

    PrintByFirst("p1", my_pair1);
    PrintByGet("p2", my_pair2);
    IsSame(my_pair1, my_pair2);

    std::pair<std::string, int> my_pair3 = my_pair1;
    PrintByFirst("p3", my_pair3);
    IsSame(my_pair3, my_pair1);
    IsMore(my_pair3, my_pair1);

    my_pair1.swap(my_pair2);
    PrintByGet("p1", my_pair1);
    PrintByGet("p2", my_pair2);


    std::pair<std::string, int> my_pair4 = std::make_pair("Hello", 555);
    PrintByFirst("p4", my_pair4);

    std::cout << "Print pair using a template method: " << my_pair1 << std::endl;

//    bool is_equal = (my_pair1 == my_pair2) 
//    bool is_less  = (my_pair1 < my_pair2) 
//    std::cout << "p1 == p2: " << is_equal << std::endl;
//    std::cout << "p1 < p2: " << is_less << std::endl;
}
