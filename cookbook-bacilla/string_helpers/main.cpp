#include <string>
#include <algorithm>

#include <iostream>

using namespace std::string_literals;

namespace string_library
{
    template <typename CharT>
    using tstring = std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT>>;

    template <typename CharT>
    using tstringstream = std::basic_stringstream<CharT, std::char_traits<CharT>, std::allocator<CharT>>;
    
    
    template<typename CharT>
    inline tstring<CharT> to_upper(tstring<CharT> text)
    {
        std::transform(std::begin(text), std::end(text), std::begin(text), toupper);
        return text;
    }
    
    
    template<typename CharT>
    inline tstring<CharT> to_lower(tstring<CharT> text)
    {
        std::transform(std::begin(text), std::end(text), std::begin(text), tolower);
        return text;
    }

   
    template<typename CharT>
    inline tstring<CharT> reverse(tstring<CharT> text)
    {
        std::reverse(std::begin(text), std::end(text));
        return text;
    } 
    
    
    template<typename CharT>
    inline tstring<CharT> trim(tstring<CharT> const & text)
    {
        auto first { text.find_first_not_of(' ') };
        auto last { text.find_last_not_of(' ' ) };
        return text.substr(first, (last-first + 1));
    }
    
    template<typename CharT>
    inline tstring<CharT> trimleft(tstring<CharT> const & text)
    {
        auto first { text.find_first_not_of(' ') };
        return text.substr(first, text.size() - first);
    }
    
    template<typename CharT>
    inline tstring<CharT> trimright(tstring<CharT> const & text)
    {
        auto last { text.find_last_not_of(' ') };
        return text.substr(0, last+1);
    }
}    
    
int main()
{
    auto ut { string_library::to_upper("this is not UPPERCASE"s) };
    std::cout << ut << std::endl;
    auto lt { string_library::to_lower("THIS IS NOT LOWERCASE"s) };
    std::cout << lt << std::endl;
    
    auto rt { string_library::reverse("reverse this"s) };
    std::cout << rt << std::endl;
    
    auto text1 { "             this is an example      "s};
    auto t1 { string_library::trim(text1) };
    auto t2 { string_library::trimleft(text1) };
    auto t3 { string_library::trimright(text1) };
    
    std::cout << "|" << text1 << "|" << std::endl;
    std::cout << "|" << t1 << "|" << std::endl;
    std::cout << "|" << t2 << "|" << std::endl;
    std::cout << "|" << t3 << "|" << std::endl; 
}
