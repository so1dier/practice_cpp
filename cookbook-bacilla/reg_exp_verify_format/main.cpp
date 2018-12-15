#include <regex>
#include <string>

#include <iostream>
#include <iomanip>

using namespace std::string_literals;



bool is_valid_email_format(std::string const & email)
{
    auto pattern {R"(^[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z]{2,}$)"s};
    auto rx = std::regex{pattern, std::regex_constants::icase};
    return std::regex_match(email, rx);
}

int main()
{
//    auto email_pattern = {R"(^[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z{2,}$)"s};
//    auto rx = std::regex{email_pattern};
//    auto rx = std::regex{email_pattern, std::regex_constants::icase};
//    auto valid = std::regex_match("don.aus@gmail.com"s, rx);



    auto ltest = [](std::string const & email)
    {
        std::cout << std::setw(30) << std::left
        << email << " : "
        << (is_valid_email_format(email) ?
        "valid format" : "invalid format")
        << std::endl;
    };

    ltest("JOHN.DOE@DOMAIN.COM"s); // valid format
    ltest("JOHNDOE@DOMAIL.CO.UK"s); // valid format
    ltest("JOHNDOE@DOMAIL.INFO"s); // valid format
    ltest("J.O.H.N_D.O.E@DOMAIN.INFO"s); // valid format
    ltest("ROOT@LOCALHOST"s); // invalid format
    ltest("john.doe@domain.com"s); // invalid format
    
    /*
    std::cout << "JOHN.DOE@DOMAIN.COM ";
    if (is_valid_email_format("JOHN.DOE@DOMAIN.COM"))
    {
        std::cout << " valid";
    }
    else
    {
        std::cout << " NOT valid";
    }
    std::cout << std::endl;
    */
}
