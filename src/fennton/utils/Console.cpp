#ifdef _WIN32
#include <Windows.h>
#endif

#include <fennton/utils/Console.hpp>

namespace Fennton::Console {
    #ifdef _WIN32
    static std::optional<decltype(GetConsoleOutputCP())> lastCP;
    #endif

    // The default stream used by the print* functions.
    // For now it's always std::cout.
    static std::ostream* defaultStream = &std::cout;

    void init() {
        #ifdef _WIN32
        // Saves the previous codepage.
        lastCP = GetConsoleOutputCP();
        // Changes the console's codepage to support UTF-8.
        SetConsoleOutputCP(CP_UTF8);
        #endif
    }
    void term() {
        #ifdef _WIN32
        // Only resets the console's codepage if init was called, and thus the last codepage 
        // saved.
        if (lastCP) {
            // Reverts the console's codepage to the previous one.
            SetConsoleOutputCP(*lastCP);
            // Resets lastCP to handle the console's previous codepage 
            // changing after a previous termination or initialisation.
            lastCP = {};
        }
        #endif
    }
}