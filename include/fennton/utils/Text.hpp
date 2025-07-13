#include <string>

namespace Fennton::Text {
    // Returns the string with all the characters which need escaping replaced with their C++ 
    // escape sequences.
    std::string escape(std::string const& str);
    // Returns the string inside double quotes and with all characters which need escaping 
    // replaced with their C++ escape sequences.
    std::string quote(std::string const& str);
}