#include <iostream>
#include <string>
#include <vector>
#include <memory>

int main()
{
    std::shared_ptr<std::string> pDan(new std::string("Daniel"));
    std::shared_ptr<std::string> pGol(new std::string("Gol"));

    (*pDan)[0] = 'd';
    pGol->replace(0,1,"g");

    std::vector<std::shared_ptr<std::string>> who_made_coffee;
    who_made_coffee.push_back(pDan);
    who_made_coffee.push_back(pGol);
    who_made_coffee.push_back(pDan);
    who_made_coffee.push_back(pGol);

    for(auto ptr : who_made_coffee)
    {
        std::cout << *ptr << " ";
    }
    std::cout << std::endl;

    *pDan = "Danski";

    std::cout << "Printing all elements after a change..." << std::endl;
    for(auto ptr : who_made_coffee)
    {
        std::cout << *ptr << " ";
    }
    std::cout << std::endl;

    std::cout << "use_cout: " << who_made_coffee[0].use_count() << std::endl;

    {
        std::shared_ptr<std::string> name(new std::string("Madrido"), [](std::string* message) { std::cout << "Deleting " << *message << std::endl;
            delete message; });

    }
    {
        //make_shared does not have a custom deleter to pass on
        //auto name = make_shared(new std::string("Madrido"), deleter); 
        auto name = std::make_shared<std::string>("Madrido2");
        std::cout << name << " " << *name << std::endl;
    }
    {
        auto name = std::make_shared<std::string>("Barcelona");
        name.reset(new std::string("Valencia"));
        std::cout << *name << std::endl;
    }

    {
        std::shared_ptr<int> p(new int[10], [](int* p)
                { std::cout << " deleting p int[10]\n"; delete[] p; });

    }

    

}
