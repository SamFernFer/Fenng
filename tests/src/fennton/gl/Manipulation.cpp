#include <fennton/gl/Window.hpp>
#include <fennton/utils/Console.hpp>
#include <fennton/utils/Text.hpp>
#include <glad/glad.h>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <thread>
#include <string>
#include <locale>
#include <atomic>
#include <algorithm>
#include <cstdint>

namespace Console = Fennton::Console;
namespace Text = Fennton::Text;
namespace Gl = Fennton::Gl;

using namespace Fennton::Memory;
using Fennton::Gl::Window;

enum class TestResult {
    None = 0,
    Pass = 1,
    Fail = 2
};

// std::atomic<TestResult> testResult = TestResult::None;

void init();
void term();
void askForResult();
int main() {
    try {
        init();

        Strong<Window> _win = Window::create(
            800, // Initial width.
            600, // Initial height.
            "Window Manip", // Window name.
            nullptr, // Not fullscreen.
            nullptr // Not sharing its context.
        );
        // It is necessary to have a current context before initialising the graphics.
        _win->MakeContextCurrent();
        // Initialises OpenGL.
        Gl::init();

        std::thread _t1 = std::thread(askForResult);
        _t1.detach();

        while (!_win->ShouldClose()) {
            Window::pollEvents();

            if (!_win->IsIconified()) {
                glClearColor(0.1f, 0.3f, 0.15f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
            }
            _win->SwapBuffers();
        }
        _win->Destroy();

        // Console::pause();
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
    Window::term();
    Console::term();
}
void askForResult() {
    std::string _res = Console::readl();
    // Converts the string to lowercase using C locale.
    for (char& c : _res) c = std::tolower(c, std::locale::classic());

    if (_res == "fail") {
        Console::printl("The test failed.");
    } else if (_res == "pass") {
        Console::printl("The test passed.");
    } else {
        Console::printl("What?");
    }
}