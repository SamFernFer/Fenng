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
#include <unordered_set>
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
std::atomic<bool> askForDestroyResults = false;
std::atomic<bool> shouldAbortTests = false;

std::list<std::pair<std::string, bool>> testCases;
std::int32_t testCount = 0, failCount = 0;

std::unordered_set<std::string> testsToExecute;

void init();
void term();
void runTests();
bool isCaseEnabled(std::string const& testName);
void runCase(std::string const& testName, void(*stepFunc)());
void setStepFunc(void(*stepFunc)());
void askForResult(std::string const& testName);
void setResult(std::string const& testName, bool success);
int main(int argc, char** argv) {
    try {
        // Allows for executing only the specified tests, or all of them if none is specified.
        for (int i = 1; i < argc; ++i) {
            testsToExecute.emplace(argv[i]);
        }
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
                // Makes sure there is no attempt to execute a null function.
                if (_currStep) {
                    _currStep();
                }
            }
        }
        // Not need to ask for the result, as reaching this step means it already worked.
        setResult("ShouldClose", true);

        if (!askForDestroyResults.load()) {
            Console::printl();
            shouldAbortTests = true;
        }
        
        mainWindow->Destroy();

        // Not asking for the step's result when exiting early.
        if (askForDestroyResults.load() && isCaseEnabled("Destroy")) {
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
    // Console::pause();

    Window::term();
    Console::term();
}
void runTests() {
    runCase("800x600",
        nullptr // Do nothing.
    );
    runCase("Hide",
        []()->void {
            mainWindow->Hide();
        }
    );
    runCase("Show",
        []()->void {
            mainWindow->Show();
        }
    );
    runCase("Iconify",
        []()->void {
            mainWindow->Iconify();
        }
    );
    runCase("Restore from Iconify",
        []()->void {
            mainWindow->Restore();
        }
    );
    runCase("Maximise",
        []()->void {
            mainWindow->Maximise();
        }
    );
    runCase("Restore from Maximise",
        []()->void {
            mainWindow->Restore();
        }
    );
    runCase("Maximise before Iconify",
        []()->void {
            mainWindow->Maximise();
        }
    );
    runCase("Iconify after Maximise",
        []()->void {
            mainWindow->Iconify();
        }
    );
    runCase("Restore from Iconify after Maximise",
        []()->void {
            mainWindow->Restore();
        }
    );
    runCase("Restore again",
        []()->void {
            mainWindow->Restore();
        }
    );
    runCase("Fullscreen (windowed)",
        []()->void {
            mainWindow->SetMonitor(Monitor::GetPrimary());
        }
    );

    // Enables asking for the result of the "Destroy" case.
    askForDestroyResults = true;
    
    setStepFunc([]()->void {
        mainWindow->SetShouldClose(true);
    });
}
bool isCaseEnabled(std::string const& testName) {
    return testsToExecute.empty() || testsToExecute.contains(testName);
}
void runCase(std::string const& testName, TestStep stepFunc) {
    if (!isCaseEnabled(testName)) {
        return;
    }
    // Sets the function for the current test step (might be null).
    setStepFunc(stepFunc);
    // Ask if the case passed.
    askForResult(testName);
}
void setStepFunc(TestStep stepFunc) {
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
    if (!success) {
        ++failCount;
    }
    ++testCount;
    testCases.emplace_back(std::make_pair(testName, success));
}