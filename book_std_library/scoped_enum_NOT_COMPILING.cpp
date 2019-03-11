#include <iostream>

enum class Status : char { Good, Bad, Unknown };


int main()
{
    Status status = Status::Good;
    std::cout << "status " << status;

}
