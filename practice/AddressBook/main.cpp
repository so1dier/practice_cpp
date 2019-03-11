#include <iostream>
#include <fstream>
#include <string>

class Person
{
public:
    Person(const std::string& name, const std::string& number) : m_name(name), m_number(number) {}
    ~Person(){}

    std::string Name() { return m_name; }
    std::string Number() { return m_number; }

    friend std::ostream& operator<< (std::ostream& out, const Person& person);

private:
    std::string m_name;
    std::string m_number;
};

std::ostream& operator<< (std::ostream& out, const Person& person)
{
    out << "name = " << person.m_name;
    out << " number = " << person.m_number;
    return out;
}

int main()
{
    Person person("Dan", "1111");

    std::cout << person << std::endl;
    std::ofstream file("book.txt");
    if (file.is_open())
    {
        file << person;
    }
    else
    {
        std::cout << "Unable to open the file" << std::endl;
    }
}
