#include <future>
#include <mutex>
#include <iostream>
#include <string>


std::mutex mtx;

void print(const std::string& s)
{
    std::lock_guard<std::mutex> lock(mtx);
    for (char c : s)
    {
        std::cout.put(c);
    }
    std::cout << std::endl;
}

int main()
{
    auto f1 = std::async(std::launch::async, print, "Hello from a first thread");
    auto f2 = std::async(std::launch::async, print, "Hello from a second thread");
    print("Hello from the main thread");
}
