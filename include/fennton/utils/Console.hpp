#ifdef _WIN32
#include <Windows.h>
#endif

#include <iostream>

namespace Fennton::Console {
    decltype(GetConsoleOutputCP()) lastCP;

    // Uses RAII to make sure the codepage is reset to the previous one before the program 
    // exits. NOTE: should be used before using the Fennton::Console namespace even if the 
    // system is not Windows, as other variables might be initialised with it.
    void init() {
        
        // Saves the previous codepage.
        CPSetter() : lastCP(GetConsoleOutputCP()) {

            #ifdef _WIN32
            // Changes the console's codepage to support UTF-8.
            SetConsoleOutputCP(CP_UTF8);
            #endif
        }
        ~CPSetter() {
            // Reverts the console's codepage to the previous one.
            SetConsoleOutputCP(lastCP);
        }
    }

    // Terminates the console module.
    void term() {
        d
    }
}