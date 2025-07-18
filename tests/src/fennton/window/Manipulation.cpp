#include <fennton/utils/Console.hpp>

namespace Console = Fennton::Console;
int main() {
    std::string _msg = Console::readl();
    Console::printl("Message: ", _msg);
    Console::pausel();
    return 0;
}