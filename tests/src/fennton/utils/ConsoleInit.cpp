#include <fennton/utils/Console.hpp>

int main() {
    constexpr char const
        *line1 = "This is ASCII text.\n",
        *line2 = "Ábaco, Cristóvão, Horário, Müller, Niño, Ýúúý, どうもありがとう\n"
    ;
    // - Input: ASCII and non-ASCII input to the std::cout;
    // - Expected output (manually checked): ASCII is perfect, but non-ASCII text might be broken;
    std::cout << line1;
    std::cout << line2;

    Fennton::Console::init();

    // - Input: ASCII and non-ASCII input to the std::cout;
    // - Expected output (manually checked): all input is rendered perfectly, except characters 
    // not present in the console's font;
    std::cout << line1;
    std::cout << line2;

    Fennton::Console::term();

    // - Input: ASCII and non-ASCII input to the std::cout;
    // - Expected output (manually checked): ASCII is perfect, but non-ASCII text might be broken, 
    // exactly how it was before the call to Fennton::Console::init;
    std::cout << line1;
    std::cout << line2;

    // Pauses the console.
    std::cin.get();

    return 0;
}