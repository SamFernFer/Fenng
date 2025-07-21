#include <fennton/gl/Window.hpp>
#include <fennton/utils/Console.hpp>
#include <fennton/utils/Text.hpp>
#include <thread>
#include <chrono>
#include <stdexcept>

namespace Console = Fennton::Console;
namespace Text = Fennton::Text;
using Fennton::Gl::Window;

void init();
void term();
int main() {
    try {
        init();
        auto w = Window();
        Console::pause();
    } catch (std::exception& e) {
        Console::printl("[EXCEPTION] {}", e.what());
    } catch (...) {
        Console::printl("[UNKNOWN EXCEPTION]");
    }
    term();
    return 0;
}
void init() {
    Console::init();
    Window::init();
}
void term() {
    Console::term();
    Window::term();
}