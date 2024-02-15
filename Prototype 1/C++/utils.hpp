#ifndef COLORMOD_INCLUDED
#define COLORMOD_INCLUDED

#include <ostream>
#include <locale>
#include <string>
#include <cstddef>

class SpaceSeparator: public std::numpunct<char> // https://stackoverflow.com/a/2648663/11253795
{
public:
    SpaceSeparator(std::size_t refs): std::numpunct<char>(refs) {}
protected:
    char do_thousands_sep() const { return ' '; }
    std::string do_grouping() const { return "\03"; }
};

namespace Color { // https://stackoverflow.com/a/17469726/11253795
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


#endif // COLORMOD_INCLUDED