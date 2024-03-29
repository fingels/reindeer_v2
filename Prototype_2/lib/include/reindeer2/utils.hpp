#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED

#include <ostream>
#include <locale>
#include <string>
#include <cstddef>

/** @file utils.hpp
 *  @defgroup utils
 *  @author Florian Ingels
 * */



/**
 * @ingroup utils
 * @brief Allows to display large numbers by seperating groups of 3 numbers
 * @remark Borrowed from https://stackoverflow.com/a/2648663/11253795
*/
class SpaceSeparator: public std::numpunct<char>
{
public:
    SpaceSeparator(std::size_t refs): std::numpunct<char>(refs) {}
protected:
    char do_thousands_sep() const { return ' '; }
    std::string do_grouping() const { return "\03"; }
};


/**
 * @ingroup utils
 * @brief Allows to output colored text
 * @remark Borrowed from https://stackoverflow.com/a/17469726/11253795
*/
namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
}

/**
* @brief Hash function for unsigned long long int
* @ingroup utils
*/
uint64_t xorshift ( uint64_t x );


#endif // UTILS_INCLUDED
