#include <iostream>
#include <optional>
#include <format>

namespace Fennton::Console {
    class Printer {
    private:
        std::ostream& out;
    public:
        Printer(std::ostream& out);
        Printer(Printer const&) = delete;
        Printer(Printer&&) = delete;
        Printer& operator=(Printer const&) = delete;
        Printer& operator=(Printer&&) = delete;
        ~Printer() = default;

        // Prints the value to the output stream.
        template<typename T> void print(T const& v) {
            out << v << std::flush;
        }

        // Prints a line break.
        void printl();

        // Prints the value to the output stream followed by a line break.
        template<typename T> void printl(T const& v) {
            out << v << std::endl;
        }

        // Prints the formatted value to the output stream.
        template<typename... A> void print(std::string_view fmt, A&&... args) {
            out << std::vformat(fmt, std::make_format_args(args...)) << std::flush;
        }

        // Prints the formatted value to the output stream.
        template<typename... A> void printl(std::string_view fmt, A&&... args) {
            out << std::vformat(fmt, std::make_format_args(args...)) << std::endl;
        }
        // Returns the stream.
        std::ostream& getStream();
    };

    extern Printer defaultPrinter;

    // Uses RAII to make sure the codepage is reset to the previous one before the program 
    // exits. NOTE: should be used before using the Fennton::Console namespace even if the 
    // system is not Windows, as other variables might be initialised with it.
    void init();

    // Terminates the console module.
    void term();

    // Pauses the console until the Enter key is pressed.
    void pause();

    // Pauses the console until the Enter key is pressed, after showing the specified message.
    void pause(std::string_view msg);

    // Returns the default printer.
    Printer& getDefaultPrinter();

    // [Not implemented yet]
    // void setDefaultStream();

    // Prints the value.
    template<typename T> void print(T const& v) {
        defaultPrinter.print(v);
    }

    // Prints a line break.
    void printl();

    // Prints the value to the output stream followed by a line break.
    template<typename T> void printl(T const& v) {
        defaultPrinter.printl(v);
    }

    // Prints the formatted value to the output stream.
    template<typename... A> void print(std::string_view fmt, A&&... args) {
        defaultPrinter.print(fmt, args...);
    }

    // Prints the formatted value to the output stream.
    template<typename... A> void printl(std::string_view fmt, A&&... args) {
        defaultPrinter.printl(fmt, args...);
    }
}