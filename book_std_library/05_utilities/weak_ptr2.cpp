#include <memory>
#include <string>
#include <iostream>
#include <vector>

class Person
{
public:
    std::string  name;
    std::shared_ptr<Person> mother;
    std::shared_ptr<Person> father;
    //std::vector<std::shared_ptr<Person>> kids;
    std::vector<std::weak_ptr<Person>> kids;

    Person(const std::string& n,
           std::shared_ptr<Person> m=nullptr,
           std::shared_ptr<Person> f=nullptr)
        : name(n), mother(m), father(f)
    {
    }

    ~Person()
    {
        std::cout << "delete " << name << std::endl;
    }
};

std::shared_ptr<Person> initFamily(const std::string& name)
{
    std::shared_ptr<Person> mom(new Person(name+"'s mom"));
    std::shared_ptr<Person> dad(new Person(name+"'s dad"));
    std::shared_ptr<Person> kid(new Person(name, mom, dad));

    mom->kids.push_back(kid);
    dad->kids.push_back(kid);

    return kid;
}

int main()
{
    std::shared_ptr<Person> p = initFamily("Daniel");

    std::cout << "Daniel's family" << std::endl;
    std::cout << "Shared: " << p.use_count() << " times" << std::endl;
    if (p->mother->kids[0].expired())
    {
        std::cout << "- name of 1st kid of Daniels mom: " << p->mother->kids[0].lock()->name << std::endl;
    }
    else
    {
        std::cout << "Kids expired" << std::endl;
    }

    p = initFamily("Jim");
    std::cout << "jim's family exists" << std::endl;
}


