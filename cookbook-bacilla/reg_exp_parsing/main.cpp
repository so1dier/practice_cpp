#include <regex>
#include <string>

#include <iostream>

using namespace std::string_literals;


void first_way()
{
    
}




int main()
{
    auto text {
        R"(
timeout=120
#remove # to uncomment the following lines
server = 127.0.0.1
#retrycount=3
        )"s};

    //auto pattern { R"(^(?!#)(\w+)\s*=\s*([\w\d]+[\w\d._,\-:]*)$)"s};
    //auto pattern { R"(^(?!#)(\w+)\s*=\s*([\w\d]+[\w\d._,\-:]*)$)"s};
    auto pattern { R"((?!#)(\w+)\s*=\s*([\w\d]+[\w\d._,\-:]*))"s};

    auto rx = std::regex { pattern };
    
    //1
    std::cout << std::endl << 1 << std::endl;
    auto match = std::smatch{};
    if (std::regex_search(text, match, rx))
    {
        std::cout << match[1] << '=' << match[2] << std::endl;
    }
    
    
    //2
    std::cout << std::endl << 2 << std::endl;
    auto end1 = std::sregex_iterator{};
    for (auto it=std::sregex_iterator{ std::begin(text), std::end(text), rx };
         it != end1; 
         ++it)
    {
        std::cout << (*it)[1] << "=" << (*it)[2] << std::endl;
    }
    
    //3
    std::cout << std::endl << 3 << std::endl;
    auto end2 = std::sregex_token_iterator{}; //declared above
    for (auto it = std::sregex_token_iterator{
         std::begin(text), std::end(text), rx };
         it != end2; 
         ++it)
    {
        std::cout << *it << std::endl;
    }

}
