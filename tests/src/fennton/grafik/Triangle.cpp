#include <fennton/grafik/Window.hpp>
#include <fennton/utils/Console.hpp>
#include <fennton/utils/Text.hpp>

#include <exception>

namespace Console = Fennton::Console;
namespace Text = Fennton::Text;
namespace Grafik = Fennton::Grafik;

using Grafik::Window;
using Grafik::Monitor;

void init();
void term();

int main() {
    try {
        init();
    } catch (std::exception& e) {
        Console::printl("[EXCEPTION] {}", e.what());
    } catch (...) {
        Console::printl("[UNKNOWN EXCEPTION]");
    }
    term();
}

void init() {
    Console::init();
    Window::init();
    Monitor::init();
}
void term() {
    Monitor::term();
    Window::term();
    Console::term();
}