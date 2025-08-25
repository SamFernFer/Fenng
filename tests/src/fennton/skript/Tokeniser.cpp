#include <fennton/utils/Console.hpp>
#include <fennton/utils/Text.hpp>
#include <fennton/skript/Parser.hpp>
#include <algorithm>
#include <iterator>
#include <utility>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cstdint>

namespace Console = Fennton::Console;
namespace Text = Fennton::Text;

using Fennton::Skript::Tokeniser::Token;
using Fennton::Skript::Tokeniser::Name;
using Fennton::Skript::Tokeniser::Number;
using Fennton::Skript::Tokeniser::Punct;

static std::int64_t testCount = 0, failCount = 0;

template<typename T, typename... A> tk() {
    return f
}

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
    testCase("123", Number( { "123" }, 10, {} ));
    #if 0
    // Integers:

    // - Literals:
    testCase("0", N( "0" ));

    // - Basic arithmetics:
    testCase("-20", "-20");
    testCase("2 + 2", "4");
    testCase("2 + -2", "0");
    testCase("2 - 2", "0");
    testCase("2 -2", "0");
    testCase("20 + 4 * 0", "20");
    testCase("(20 + 4) * 0", "0");

    // - Basic comparisons:
    testCase("20 < 10", "#false");
    testCase("10 > 20", "#false");
    testCase("0 < 2 != #false", "#true");
    testCase("#false != 0 < 2", "#true");

    // - Comparison and arithmetics:
    testCase("2 + 2 != 5", "#true");
    #endif
}
void testCase(std::string const& input, std::deque<Token> const& expected) {
    ++testCount;

    std::deque<Token> const _actual = Fennton::Skript::tokenise(input);

    auto _mismatch = std::mismatch(
        _actual.begin(), _actual.end(),
        expected.begin(), expected.end()
    );

    using ItType = decltype(_mismatch.first);

    auto _listElems = [](ItType _begin, ItType _end)->std::string {
        std::stringstream _ss;
        if (_begin != _end) {
            _ss << " ";
            _ss << _begin->GetSpelling();
            for (
                auto it = std::next(_begin);
                it != _end;
                ++it
            ) {
                _ss << ", ";
                _ss << it->GetSpelling();
            }
            _ss << " ";
            return _ss.str();
        } else {
            return {};
        }
    };

    if (_mismatch.first != _mismatch.second) {
        Console::printl("[FAIL] Index {}", std::distance(_actual.begin(), _mismatch.first));
        Console::printl("[ACTUAL] [{}]", _listElems(_mismatch.first, _actual.end()));
        Console::printl("[EXPECTED] [{}]", _listElems(_mismatch.second, expected.end()));
        ++failCount;
    }
}