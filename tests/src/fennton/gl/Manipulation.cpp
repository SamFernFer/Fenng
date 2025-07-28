#include <fennton/gl/Window.hpp>
#include <fennton/utils/Console.hpp>
#include <fennton/utils/Text.hpp>
#include <glad/glad.h>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <thread>
#include <string>
#include <list>
#include <utility>
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
using Ms = std::chrono::milliseconds;

typedef void(*TestStep)();

Strong<Window> mainWindow = nullptr;
TestStep lastStep = nullptr;
std::atomic<TestStep> testStep = nullptr;

std::list<std::pair<std::string, bool>> testCases;
std::int32_t testCount = 0, failCount = 0;

void init();
void term();
void runTests();
void runCase(std::string const& testName, void(*stepFunc)());
void askForResult(std::string const& testName);
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

        std::size_t _largestSize = 0;
        for (auto& _case : testCases) {
            _largestSize = std::max(_largestSize, _case.first.size());
        }
        for (auto& _case : testCases) {
            Console::printl(
                "{:>{}}: {}",
                _case.first,
                _largestSize,
                _case.second? "PASS" : "FAIL"
            );
        }

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
    Window::term();
    Console::term();

    Console::pause();
}
void runTests() {
    while (true) {
        auto _hideFunc = []()->void {
            mainWindow->Hide();
        };
        auto _showFunc = []()->void {
            mainWindow->Hide();
        };
        auto _closeFunc = []()->void {
            mainWindow->SetShouldClose(true);
        };

        // Do nothing.
        // Wait some seconds so the user can check if the results are okay.
        std::this_thread::sleep_for(Ms(2000));
        // Hide the window.
        testStep = _hideFunc;
        // Ask if the case passed.
        askForResult("800x600");
        // Reshow the window.
        testStep = _showFunc;


        std::this_thread::sleep_for(Ms(2000));
        testStep = _hideFunc;
        askForResult("800x600");
        testStep = _showFunc;


        // Hide window.
        

        // Repeat.

        std::this_thread::sleep_for(Ms(2000));
        testStep = _closeFunc;
    }
}
void runCase(std::string const& testName, void(*stepFunc)()) {
}
void askForResult(std::string const& testName) {
    std::string _res = Console::readl();
    // Converts the string to lowercase using the C locale.
    for (char& c : _res) c = std::tolower(c, std::locale::classic());

    decltype(testCases)::reference _case =
        testCases.emplace_back(std::make_pair(testName, false))
    ;
    if (_res == "pass" || _res == "p") {
        _case.second = true;
    }
}