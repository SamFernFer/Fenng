#include <fennton/utils/Console.hpp>

int main() {
    // - Input: ASCII and non-ASCII input to the std::cout;
    // - Expected output (manually checked): ASCII is perfect, but non-ASCII text might be broken;
    std::cout << "This is ASCII text.\n";
    std::cout << "Ábaco, Cristóvão, Horário, Müller, Niño, Ýúúý\n";

    Fennton::Console::init();

    // - Input: ASCII and non-ASCII input to the std::cout;
    // - Expected output (manually checked): all input is rendered perfectly, except characters 
    // not present in the console's font;
    std::cout << "This is ASCII text.\n";
    std::cout << "Ábaco, Cristóvão, Horário, Müller, Niño, Ýúúý\n";

    Fennton::Console::term();

    // - Input: ASCII and non-ASCII input to the std::cout;
    // - Expected output (manually checked): ASCII is perfect, but non-ASCII text might be broken, 
    // exactly how it was before the call to Fennton::Console::init;
    std::cout << "This is ASCII text.\n";
    std::cout << "Ábaco, Cristóvão, Horário, Müller, Niño, Ýúúý\n";

    // Pauses the console.
    std::cin.get();

    return 0;
}