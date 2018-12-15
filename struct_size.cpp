#include <cstdio>
#include <bitset>
#include <iostream>

struct A
{
   char a;
};
struct B
{
   char b;
   int HelloWorld(){ int i = 1000; printf("%d", i); }
};

struct alignas(16) C
{
   char c;
};


void GetBitset(const int i)
{
    std::bitset<16> b(i);
    std::cout << b << std::endl;
}

int main()
{
    A a;
    B b;
    C c;

    int size_a = sizeof(a);
    int size_b = sizeof(b);
    int size_c = sizeof(c);

    printf("%u, 0x%08x ",size_a, size_a);
    GetBitset(size_a);
    printf("%u, 0x%08x ",size_b, size_b);
    GetBitset(size_b);
    printf("%u, 0x%08x ",size_c, size_c);
    GetBitset(size_c);
}
