#include <iostream>

class P
{
    public:
        P(int a, int b){};
        explicit P(int a, int b, int c){};
};
        

void PrintAll(const std::initializer_list<int>& list)
{
    for (auto& elem : list)
    {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

int main()
{
    PrintAll({1,2,3,4,5});

    P x(77,5); //OK
    P y{77,5}; //OK
    P z{77,5, 42}; //OK
    P v = {77,5}; //ok, implicit type conversion allowed
    //P w = {77,5, 42}; //Error due to explicit conversion not allowed

}
