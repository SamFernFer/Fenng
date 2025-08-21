#include <fennton/utils/Console.hpp>
#include <fennton/utils/Text.hpp>
#include <fennton/skript/Parser.hpp>
#include <stdexcept>
#include <cstdint>

namespace Console = Fennton::Console;
namespace Text = Fennton::Text;

static std::int64_t testCount = 0, failCount = 0;

void init();
void term();
void runTests();
void testCase();
int main(int argc, char** argv) {
    try {
        init();
        runTests();
        // Prints the total number of failures.
        Console::printl("[TOTAL] {}/{} tests failed.", failCount, testCount);
        Console::printl("[RESULT] {}", failCount == 0? "PASS" : "FAIL");
    } catch (std::exception& e) {
        Console::printl("[EXCEPTION] {}", e.what());
    } catch (...) {
        Console::printl("[UNKNOWN EXCEPTION]");
    }
    term();
}
void init() {
    Console::init();
}
void term() {
    Console::pause();
    Console::term();
}
void runTests() {
    testCase("", { });
}
void testCase() {
    d
}