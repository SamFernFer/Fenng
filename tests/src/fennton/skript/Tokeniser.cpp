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
using Fennton::Skript::Tokeniser::tokenise;

static std::int64_t testCount = 0, failCount = 0;

void init();
void term();
void runTests();
void testSpelling(Token const& token, std::string_view expected);
void testTokens(std::string const& input, std::deque<Token> const& expected);
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
    testSpelling(Number( { "123" }, {}, 10), "123");

    /* testTokens("123", { Number( { "123" }, {}, 10) });
    testTokens("123u8", { Number( { "123" }, { "u8" }, 10) });
    testTokens("123u1", { Number( { "123" }, { "u1" }, 10) });
    testTokens("123a'b'c", { Number( { "123" }, { "a", "b", "c" }, 10) }); */
    #if 0
    // Integers:

    // - Literals:
    testTokens("0", N( "0" ));

    // - Basic arithmetics:
    testTokens("-20", "-20");
    testTokens("2 + 2", "4");
    testTokens("2 + -2", "0");
    testTokens("2 - 2", "0");
    testTokens("2 -2", "0");
    testTokens("20 + 4 * 0", "20");
    testTokens("(20 + 4) * 0", "0");

    // - Basic comparisons:
    testTokens("20 < 10", "#false");
    testTokens("10 > 20", "#false");
    testTokens("0 < 2 != #false", "#true");
    testTokens("#false != 0 < 2", "#true");

    // - Comparison and arithmetics:
    testTokens("2 + 2 != 5", "#true");
    #endif
}
void testSpelling(Token const& token, std::string_view expected) {
    ++testCount;

    std::string _actual = token.GetSpelling();
    if (_actual != expected) {
        // Prints the zero-based test index.
        Console::printl("[FAIL] Test {}", testCount - 1);
        Console::printl("[ACTUAL] [{}]", _actual);
        Console::printl("[EXPECTED] [{}]", expected);
        ++failCount;
    }
}
void testTokens(std::string const& input, std::deque<Token> const& expected) {
    ++testCount;

    std::deque<Token> const _actual = tokenise(input);

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

    if (
        _mismatch.first != _actual.end()
        || _mismatch.second != expected.end()
    ) {
        Console::printl("[FAIL] Test {} | Token {}",
            testCount - 1, // Zero-based index of the test.
            std::distance(_actual.begin(), _mismatch.first) // Index of the divergent token.
        );
        Console::printl("[INPUT] {}", Text::quote(input));
        Console::printl("[ACTUAL] [{}]", _listElems(_mismatch.first, _actual.end()));
        Console::printl("[EXPECTED] [{}]", _listElems(_mismatch.second, expected.end()));
        ++failCount;
    }
}