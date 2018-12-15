#include <list>
#include <iostream>


std::ostream& operator<<(std::ostream& ostr, const std::list<int>& list)
{
    for (auto& i : list )
    {
        ostr << " " << i;
    }
    return ostr;
}

int main()
{
    std::list<int> ls1 = {1,2,3,4,5,6,7};
    std::list<int> ls2 = {111,222,333,444,555,666,777};

    //ls1.splice(ls2.end(), ls2, ls1.begin()); 
    for ( auto& e : ls1)
    {
        //ls1.splice(ls2.begin(), ls2, ls1.begin()); 
        ls1.splice(ls2.begin(), ls1, *e);
    }

    std::cout << "ls1=" << ls1 << std::endl;
    std::cout << "ls2=" << ls2 << std::endl;
}
