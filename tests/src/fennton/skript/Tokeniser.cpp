#include <fennton/utils/Console.hpp>
#include <fennton/utils/Text.hpp>
#include <fennton/skript/Parser.hpp>
#include <stdexcept>
#include <algorithm>
#include <utility>
#include <cstdint>

namespace Console = Fennton::Console;
namespace Text = Fennton::Text;

using Fennton::Skript::Tokeniser::Token;
using Fennton::Skript::Tokeniser::Name;
using Fennton::Skript::Tokeniser::Number;
using Fennton::Skript::Tokeniser::Punct;

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
    #if 0
    // Integers:

    // - Literals:
    testCase("123", Number( "123" ));
    testCase("0", N( "0" ));

    // - Basic arithmetics:
    testCase("-20", "-20");
    testCase("2 + 2", "4");
    testCase("20 + 4 * 0", "20");
    testCase("(20 + 4) * 0", "0");

    // - Basic comparisons:
    testCase("20 < 10", "#false");
    testCase("0 < 2 != #false", "#true");

    // - Comparison and arithmetics:
    testCase("2 + 2 != 5", "#true");
    #endif
}
void testCase(std::string const& input, std::deque<Token> const& expected) {
    ++testCount;

    std::deque<Token> _actual = Fennton::Skript::tokenise(input);

    auto _mismatch = std::mismatch(
        _actual.begin(), _actual.end(),
        expected.begin(), expected.end()
    );

    if (_mismatch.first != _mismatch.second) {
        ++failCount;
    }
}