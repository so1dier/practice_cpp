#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <utility>


void IsNumberPresent(const std::vector<int>& A, int number_to_search)
{
    bool present = std::binary_search(A.begin(), A.end(), number_to_search); //true
    std::cout << number_to_search << " is " << present << std::endl;
}

void PrintVector(const std::vector<int>& A)
{
    for (auto&& a : A)
    {
        std::cout << a << ' ';
    }
    std::cout << std::endl;
}

bool CustomComparator(int x, int y)
{
    return x>y;
}

void VectorDemo()
{
    std::vector<int> A { 11,2,3,14, 15 };

    std::cout << A[1] << std::endl;

    std::sort (A.begin(), A.end()); // 0(NlogN)

    PrintVector(A);

    //0(logN) search on a sorted container
    IsNumberPresent(A, 3);
    IsNumberPresent(A, 4);

    A.push_back(100);
    A.push_back(100);
    A.push_back(100);
    A.push_back(100);
    A.push_back(100);
    A.push_back(123);

    PrintVector(A);

    std::vector<int>::iterator it1 = std::lower_bound(A.begin(),
                                                 A.end(),
                                                 100);
    auto it2 = std::upper_bound(A.begin(),
                                                 A.end(),
                                                 100);

    std::cout << *it1 << " " << *it2 << std::endl;
    std::cout << it2 - it1 << " elements of 100 is in container" << std::endl;


    //reverse sort with custom comparator function
    std::sort(A.begin(), A.end(), CustomComparator);
    PrintVector(A);

    for (int& x : A)
    {
        x++;
        std::cout << x << " ";
    }
    std::cout << std::endl;
    PrintVector(A);
}



void SetDemo()
{
    std::set<int> S;
    S.insert(1);
    S.insert(2);
    S.insert(-1);
    S.insert(-10);

    for (int x : S)
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    auto it = S.find(-1);
    if (it == S.end())
    {
        std::cout << *it << " element not present" << std::endl;
    }
    else
    {
        std::cout << *it << " present" << std::endl;
    }

    auto it2 = S.upper_bound(-1);
    auto it3 = S.upper_bound(0);
    std::cout << *it2 << " " << *it3 << std::endl;

    auto it4 = S.upper_bound(2);
    if (it4 == S.end())
    {
        std::cout << *it4 << " oops! not found" << std::endl;
    }
}

void mapDemo()
{
    std::map<int, int> M;
    M[1] = 100;
    M[2] = -1;
    M[3] = 200;
    M[100000232] = 1;

    std::map<char, int> cnt;
    std::string x = "daniel golitsynski";

    for (char c : x)
    {
        cnt[c]++;
    }

    std::cout << cnt['a'] << " " << cnt['z'] << std::endl;

    for (auto p : cnt)
    {
        std::cout << p.first << "=" << p.second << "  ";// << std::endl;
    }
    std::cout << std::endl;
}

void PowerOfStl()
{
    std::set<pair<int,int>> S;

    S.insert({ 2, 3});
    S.insert({ 4, 6});
    S.insert({ 20, 333});
    S.insert({ 12121, 2343});
    S.insert({ 20, 30});

    //auto a1 = std::upper_bound(S.begin(), S.end(), {4,6});
    auto a1 = S.upper_bound({50, INT8_MAX});
}

int main()
{
    VectorDemo();
    SetDemo();
    mapDemo();
    PowerOfStl();
}


                        
