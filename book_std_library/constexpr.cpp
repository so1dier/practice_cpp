


constexpr int square(int x)
//int square(int x) //ERROR: will not work!
{
    return x*x;
}

float a[square(9)];
int main()
{
    a[0] = 12.2;
}
