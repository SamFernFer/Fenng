#include <iostream>
#include <optional>

namespace Fennton::Console {
    // Uses RAII to make sure the codepage is reset to the previous one before the program 
    // exits. NOTE: should be used before using the Fennton::Console namespace even if the 
    // system is not Windows, as other variables might be initialised with it.
    void init();

    // Terminates the console module.
    void term();

    // [Not implemented yet]
    // void setDefaultStream();

    // Prints the value to the output stream.
    template<typename T> void print(std::ostream& out, T const& v) {
        out << v;
    }
}