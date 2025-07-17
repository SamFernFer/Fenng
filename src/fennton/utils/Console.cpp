#ifdef _WIN32
#include <Windows.h>
#endif

#include <fennton/utils/Console.hpp>
#include <sstream>

namespace Fennton::Console {
    #ifdef _WIN32
    static std::optional<decltype(GetConsoleOutputCP())> lastCP;
    #endif

    // The default stream used by the print* functions.
    // For now it's always std::cout.
    static std::ostream* defaultStream = &std::cout;

    Printer defaultPrinter = Printer(std::cout);

    Printer::Printer(std::ostream& out) : out(out) {}
    void Printer::printl() {
        out << std::endl;
    }
    std::ostream& Printer::getStream() {
        return out;
    }

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
    void pause() {
        // The character type, just to make it as cross-platform as possible.
        using CharT = decltype(std::cin)::char_type;
        // The EOF character.
        CharT _eof = decltype(std::cin)::traits_type::eof();
        // Loops until stdin is clear.
        while (std::cin.get() != _eof) {}
    }
    void pause(std::string_view msg) {
        print(msg);
        pause();
    }
    void pausel() {
        printl();
        pause();
    }
    void pausel(std::string_view msg) {
        printl(msg);
        pause();
    }
    std::string readl() {
        std::stringstream _ss;
        // The character type, just to make it as cross-platform as possible.
        using CharT = decltype(std::cin)::char_type;
        // The EOF character.
        CharT _eof = decltype(std::cin)::traits_type::eof();

        while (true) {
            CharT _c = std::cin.get();
            if (_c == _eof) break;
            _ss << _c;
        }
        return _ss.str();
    }

    Printer& getDefaultPrinter() {
        return defaultPrinter;
    }
    void printl() {
        defaultPrinter.printl();
    }
}