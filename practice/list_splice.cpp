#include <list>
#include <iostream>


std::ostream& operator<<(std::ostream& out, const std::list<int>& list)
{
    for (auto& i : list)
    {
        out << i << " ";
    }
    out << std::endl;
}

int main()
{
    std::list<int> even {1,2,3,4,5,6};
    std::list<int> odd;

    std::cout << "list1: " << even;
    std::cout << "list2: " << odd;

//    for (auto iter_even = even.begin(); iter_even != even.end(); iter_even++)
//    {
//        std::cout << "splitting now" << std::endl;
//        odd.splice(odd.end(), even, iter_even);
//    }

    auto iter_even = std::find(even.begin(), even.end(), 2);
    if (iter_even != even.end())
    {
        odd.splice(odd.end(), even, iter_even);

    }


    std::cout << "list1: " << even;
    std::cout << "list2: " << odd;


}
