#include <fennton/utils/Console.hpp>
#include <fennton/utils/Text.hpp>

namespace Console = Fennton::Console;
namespace Text = Fennton::Text;
int main() {
    std::string _msg = Console::readl();
    Console::printl("Message: {}", Text::quote(_msg));
    Console::pausel();
    return 0;
}