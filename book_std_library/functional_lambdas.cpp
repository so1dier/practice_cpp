#include<functional>
#include<iostream>

std::function<int(int,int)> returnLambda()
{
    return [](int x, int y){
        return x*y;
    };
}
int main()
{
    :wq
