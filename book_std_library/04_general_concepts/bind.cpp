#include <iostream>
#include <memory>
#include <functional>
#include <future>

void func (int x, int y){ std::cout << "func() x=" << x << ", y=" << y << std::endl; }

auto l = [] (int x, int y){ std::cout << "l() x="<<x << ", y=" << y << std::endl;};

class C
{
    public:
        void operator() (int x, int y) const { std::cout << "C::operator() x=" << x << " y=" << y << std::endl;}
        void member_func(int x, int y) const {std::cout << "C::member_func() x=" << x << " y=" << y << std::endl; }
};

int main()
{
    C c;
    std::shared_ptr<C> sp(new C);

    //std::bind(func, 77, 33)();
    //std::bind(l, 88, 44)();
    //std::bind(C(), 99, 55)();
    //std::bind(&C::member_func, c, 11, 66)();
    //std::bind(&C::member_func, sp, 22, 77)();

    std::async(func, 77, 33);
    std::async(l, 88, 44);
    std::async(C(), 99, 55);
    std::async(&C::member_func, c, 11, 66);
    std::async(&C::member_func, sp, 22, 77);
}
