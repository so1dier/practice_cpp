#include <thread>
#include <iostream>

std::thread::id master_id;

void doSomething()
{
    auto id = std::this_thread::get_id();
    if (id == master_id)
    {
        std::cout << "Hello master! id=" << id << std::endl;
    }
    else
    {
        std::cout << "Hello slave! id=" << id << std::endl;
    }

}

int main()
{
    std::thread master(doSomething);
    master_id = master.get_id();

    std::thread slave(doSomething);

    master.join();
    slave.join();
}
