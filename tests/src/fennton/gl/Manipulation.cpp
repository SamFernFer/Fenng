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
using Fennton::Gl::Monitor;
using Ms = std::chrono::milliseconds;

typedef void(*TestStep)();

Strong<Window> mainWindow = nullptr;
TestStep lastStep = nullptr;
std::atomic<TestStep> testStep = nullptr;
std::atomic<bool> askForShouldCloseResults = false;
std::atomic<bool> shouldAbortTests = false;

std::list<std::pair<std::string, bool>> testCases;
std::int32_t testCount = 0, failCount = 0;

void init();
void term();
void runTests();
void runCase(std::string const& testName, void(*stepFunc)());
void setStepFunc(void(*stepFunc)());
void askForResult(std::string const& testName);
void setResult(std::string const& testName, bool success);
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
        // Not need to ask for the result, as reaching this step means it already worked.
        setResult("ShouldClose", true);

        if (!askForShouldCloseResults.load()) {
            Console::printl();
            shouldAbortTests = true;
        }
        
        mainWindow->Destroy();

        // Not asking for the step's result when exiting early.
        if (askForShouldCloseResults.load()) {
            askForResult("Destroy");
        }

        // Gets the size of the largest test case name.
        std::size_t _largestSize = 0;
        for (auto& _case : testCases) {
            _largestSize = std::max(_largestSize, _case.first.size());
        }
        // Separates the results from the lines asking for input.
        Console::printl("--------");
        // Prints the result of each test case.
        for (auto& _case : testCases) {
            Console::printl(
                "{:>{}}: {}",
                _case.first,
                _largestSize,
                _case.second? "PASS" : "FAIL"
            );
        }
        // Prints the total number of failures.
        Console::printl("[TOTAL] {}/{} tests failed.", failCount, testCount);
        Console::printl("[RESULT] {}", failCount == 0? "PASS" : "FAIL");
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
    Console::pause();

    Window::term();
    Console::term();
}
void runTests() {
    // Do nothing.
    // Ask if the case passed.
    askForResult("800x600");

    setStepFunc([]()->void {
        mainWindow->Hide();
    });
    askForResult("Hide");

    setStepFunc([]()->void {
        mainWindow->Show();
    });
    askForResult("Show");

    setStepFunc([]()->void {
        mainWindow->Iconify();
    });
    askForResult("Iconify");

    setStepFunc([]()->void {
        mainWindow->Restore();
    });
    askForResult("Restore from Iconify");

    setStepFunc([]()->void {
        mainWindow->Maximise();
    });
    askForResult("Maximise");

    setStepFunc([]()->void {
        mainWindow->Restore();
    });
    askForResult("Restore from Maximise");

    setStepFunc([]()->void {
        mainWindow->Maximise();
    });
    askForResult("Maximise before Iconify");

    setStepFunc([]()->void {
        mainWindow->Iconify();
    });
    askForResult("Iconify after Maximise");

    setStepFunc([]()->void {
        mainWindow->Restore();
    });
    askForResult("Restore from Iconify after Maximise");

    setStepFunc([]()->void {
        mainWindow->Restore();
    });
    askForResult("Restore again");

    setStepFunc([]()->void {
        mainWindow->SetMonitor(Monitor::GetPrimary());
    });
    askForResult("Fullscreen (windowed)");

    // Enables asking for the result of the ShouldClose step (before this lock ).
    askForShouldCloseResults = true;
    
    setStepFunc([]()->void {
        mainWindow->SetShouldClose(true);
    });
    // Not asking for the results here, but in the main thread.

    
}
void runCase(std::string const& testName, void(*stepFunc)()) {
    ;
}
void setStepFunc(void(*stepFunc)()) {
    if (shouldAbortTests.load()) return;

    testStep = stepFunc;
}
void askForResult(std::string const& testName) {
    if (shouldAbortTests.load()) return;

    Console::print("{}: ", testName);
    std::string _res = Console::readl();

    if (shouldAbortTests.load()) return;
    
    // Converts the string to lowercase using the C locale.
    for (char& c : _res) c = std::tolower(c, std::locale::classic());

    decltype(testCases)::reference _case =
        testCases.emplace_back(std::make_pair(testName, false))
    ;
    if (_res == "pass" || _res == "p") {
        _case.second = true;
    } else {
        ++failCount;
    }
    ++testCount;
}
void setResult(std::string const& testName, bool success) {
    testCases.emplace_back(std::make_pair(testName, success));
}