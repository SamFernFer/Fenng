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
#include <functional>
#include <algorithm>
#include <cstdint>

namespace Console = Fennton::Console;
namespace Text = Fennton::Text;
namespace Gl = Fennton::Gl;

using namespace Fennton::Memory;
using Fennton::Gl::Window;

typedef void(*TestStep)();

Strong<Window> mainWindow = nullptr;
TestStep lastStep = nullptr;
std::atomic<TestStep> testStep = nullptr;

void init();
void term();
// void runStep();
void runTests();
int main() {
    try {
        init();

        mainWindow = Window::create(
            800, // Initial width.
            600, // Initial height.
            "Window Manip", // Window name.
            nullptr, // Not fullscreen.
            nullptr // Not sharing its context.
        );
        // It is necessary to have a current context before initialising the graphics.
        mainWindow->MakeContextCurrent();
        // Initialises OpenGL.
        Gl::init();

        std::thread _t1 = std::thread(runTests);
        _t1.detach();

        while (!mainWindow->ShouldClose()) {
            Window::pollEvents();

            if (!mainWindow->IsIconified()) {
                glClearColor(0.1f, 0.3f, 0.15f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
            }
            mainWindow->SwapBuffers();

            TestStep _currStep = testStep.load();
            if (lastStep != _currStep) {
                lastStep = _currStep;
                _currStep();
            }
        }
        mainWindow->Destroy();

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
// void runStep() {
//     std::uint32_t _currStep = testStep.load();
//     if (_currStep != lastStep) {
//         switch (static_cast<TestStep>(_currStep)) {
//             case TestStep::First:
//                 break;
//             case TestStep::Final:
                
//                 break;
//         }
//         lastStep = testStep;
//     }
// }
void runTests() {
    while (true) {

        // Execute step by setting the step enum (the first one doesn't have anything.
        // Hide window.
        // Ask if it worked.

        // Repeat.
        // Show results.

        std::string _res = Console::readl();
        // Converts the string to lowercase using C locale.
        for (char& c : _res) c = std::tolower(c, std::locale::classic());

        if (_res == "fail") {
            Console::printl("The test failed.");
            testStep = []()->void {
                mainWindow->Iconify();
            };
        } else if (_res == "pass") {
            Console::printl("The test passed.");
            testStep = []()->void {
                mainWindow->Maximise();
            };
        } else {
            Console::printl("What?");
            testStep = []()->void {
                mainWindow->SetShouldClose(true);
            };
        }
    }
}