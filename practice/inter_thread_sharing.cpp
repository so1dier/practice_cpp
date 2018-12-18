#include <iostream>
#include <string>
#include <thread>


#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <string>  // string

bool is_healthy = false; 
    
std::string current_time_and_date()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

int CurrentSecondOfAMinute()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%S");

    //std::string::size_type sz;
    int sec{};// = std::stoi(second, &sz);
    ss >> sec;
    return sec;

}
    
void WorkThread1()
{
    std::cout << current_time_and_date() << " Hello world! from thread, is healthy address " << &is_healthy << std::endl;
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (CurrentSecondOfAMinute() % 2 == 0)
        {
            is_healthy = false;
        }
        else
        {
            is_healthy = true;
        }
    }
}


int main(int argc, char* argv[])
{
    std::thread th1(WorkThread1);

    std::cout << current_time_and_date() << " From main" << std::endl;

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << current_time_and_date() << " second = " << CurrentSecondOfAMinute() << " Is Healthy = " << is_healthy <<  " addr = " << &is_healthy << std::endl;
    }


    th1.join();

}
