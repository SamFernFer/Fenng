#include <fennton/utils/Console.hpp>
#include <fennton/utils/Text.hpp>
#include <fennton/skript/Parser.hpp>
#include <algorithm>
#include <iterator>
#include <utility>
#include <string>
#include <sstream>
#include <concepts>
#include <stdexcept>
#include <typeinfo>
#include <cstdint>

namespace Console = Fennton::Console;
namespace Text = Fennton::Text;

using Fennton::Skript::Tokeniser::Token;
using Fennton::Skript::Tokeniser::Name;
using Fennton::Skript::Tokeniser::Number;
using Fennton::Skript::Tokeniser::Punct;
using Fennton::Skript::Tokeniser::tokenise;
using Fennton::Skript::Tokeniser::Exception;

static std::int64_t testCount = 0, failCount = 0;

void init();
void term();
void runTests();
// Tests the spelling of a token.
void testSpelling(Token const& token, std::string_view expected);
// Tests if trying to get the spelling of a token results in the expected exception.
/* template<std::derived_from<Exception> ExceptionType>
void testSpelling(Token const& token, ExceptionType const& exception) {
    ++testCount;
    try {
        // Gets the spelling.
        std::string _spelling = token.GetSpelling();
        // Failed because there was no exception.
        Console::printl("[FAIL] Test {}", testCount - 1);
        Console::printl("[ACTUAL] Spelling: {}", Text::quote(_spelling));
        Console::printl("[EXPECTED] ");
    } catch (ExceptionType const& e) {
        // Zero-based index of the test.
        Console::printl("[FAIL] Test {}", testCount - 1);
        Console::printl("[ACTUAL] {} | {}" typeid(e).name(), e.what());
        Console::printl("[EXPECTED] {} | {}" typeid(ExceptionType).name(), exception.what());
    } catch (...) {
        // Zero-based index of the test.
        Console::printl("[FAIL] Test {}", testCount - 1);
        Console::printl("[ACTUAL] Unknown exception.");
        Console::printl("[EXPECTED] {} | {}" typeid(ExceptionType).name(), exception.what());
    }
} */
// Tests the tokenisation of a string.
void testTokens(std::string const& input, std::deque<Token> const& expected);
// Tests if trying to tokenise the string results in the expected exception.
/* template<std::derived_from<Exception> ExceptionType>
void testTokens(std::string const& input, ExceptionType const& exception) {
    ;
} */
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
    Console::printl("[SECTION] Integers - Spelling");

    testSpelling(Number( { "123" }, {}, 10), "123");
    testSpelling(Number( { "123" }, { "u8" }, 10), "123u8");
    testSpelling(Number( { "123" }, { "u1" }, 10), "123u1");
    testSpelling(Number( { "123" }, { "a", "b", "c" }, 10), "123a'b'c");

    /* Console::printl("[SECTION] Integers - Tokenisation");

    testTokens("123", { Number( { "123" }, {}, 10) });
    testTokens("123u8", { Number( { "123" }, { "u8" }, 10) });
    testTokens("123u1", { Number( { "123" }, { "u1" }, 10) });
    testTokens("123a'b'c", { Number( { "123" }, { "a", "b", "c" }, 10) }); */

    #if 0
    

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

    Console::printl("[SECTION] Decimals - Spelling");

    testSpelling(Number( { "123", "912" }, {}, 10), "123.912");
    testSpelling(Number( { "123", "912", "0", "1000" }, {}, 10), "123.912.0.1000");
    testSpelling(Number( { "123", "912", "0" }, { "u8" }, 10), "123.912.0u8");
    testSpelling(Number( { "123", "0" }, { "u1" }, 10), "123.0u1");
    testSpelling(Number( { "123", "661" }, { "a", "b", "c" }, 10), "123.661a'b'c");

    /* Console::printl("[SECTION] Decimals - Tokenisation");

    testTokens("123", { Number( { "123" }, {}, 10) });
    testTokens("123u8", { Number( { "123" }, { "u8" }, 10) });
    testTokens("123u1", { Number( { "123" }, { "u1" }, 10) });
    testTokens("123a'b'c", { Number( { "123" }, { "a", "b", "c" }, 10) }); */
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

    try {
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
    } catch (std::exception const& e) {
        // Prints the zero-based index of the test.
        Console::printl("[FAIL] Test {}", testCount - 1);
        Console::printl("[INPUT] {}", Text::quote(input));
        Console::printl("[EXCEPTION] {} | {}", typeid(e).name(), e.what());
        ++failCount;
    } catch (...) {
        ++failCount;
    }
}