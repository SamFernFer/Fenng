#include <fennton/utils/Console.hpp>

int main() {
    // TODO:
    // - Input: ASCII and non-ASCII input to the std::cout;
    // - Expected output (manually checked): ASCII is perfect, but non-ASCII text might be broken;
    Fennton::Console::init();
    // TODO:
    // - Input: ASCII and non-ASCII input to the std::cout;
    // - Expected output (manually checked): all input is rendered perfectly, except characters 
    // not present in the console's font;
    Fennton::Console::term();
    // TODO:
    // - Input: ASCII and non-ASCII input to the std::cout;
    // - Expected output (manually checked): ASCII is perfect, but non-ASCII text might be broken, 
    // exactly how it was before the call to Fennton::Console::init;
    return 0;
}