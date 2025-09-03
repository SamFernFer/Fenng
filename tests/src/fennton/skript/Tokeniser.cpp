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
#include <initializer_list>
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
Token number(
    bool hasSpaceAfter,
    std::vector<std::string_view> const& parts,
    std::vector<std::string_view> const& suffixes,
    std::int32_t base
);
void runTests();
// Tests the spelling of a token.
void testSpelling(Token::VariantType const& innerToken, std::string_view expected);
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
void testTokens(std::string const& input, std::initializer_list<Token> const& expected);
// Tests if trying to tokenise the string results in the expected exception.
/* template<std::derived_from<Exception> ExceptionType>
void testTokens(std::string const& input, ExceptionType const& exception) {
    ;
} */
int main(int argc, char** argv) {
    int _errorCode;
    try {
        init();

        // Console::printl("&_errorCode = {}", static_cast<void*>(&_errorCode));

        /* for (int i = 0; i < 128; ++i) {
            Console::printl("{} = {}", Text::quote(std::string({static_cast<char>(i)})), i);
        } */

        runTests();
        // Prints the total number of failures.
        Console::printl("[TOTAL] {}/{} tests failed.", failCount, testCount);
        Console::printl("[RESULT] {}", failCount == 0? "PASS" : "FAIL");
        _errorCode = 0;
    } catch (std::exception& e) {
        Console::printl("[EXCEPTION] {}", e.what());
        _errorCode =  0b1;
    } catch (...) {
        Console::printl("[UNKNOWN EXCEPTION]");
        _errorCode =  0b01;
    }
    term();
    return _errorCode;
}
void init() {
    Console::init();
}
void term() {
    Console::pause();
    Console::term();
}
Token number(
    bool hasSpaceAfter,
    std::vector<std::string_view> const& parts,
    std::vector<std::string_view> const& suffixes,
    std::int32_t base
) {
    return Token(Number(parts, suffixes, base), hasSpaceAfter);
}
void runTests() {
    Console::printl("[SECTION] Integers - Spelling");
    // NOTE: Not testing spellings from tokens with internal states which would never 
    // happen under normal usage.

    testSpelling(Number( { "0" }, {}, 10), "0");
    /* testSpelling(Number( { "123" }, {}, 10), "123");
    testSpelling(Number( { "10" }, {}, 8), "010");
    testSpelling(Number( { "10" }, {}, 2), "0b10");
    testSpelling(Number( { "ff" }, {}, 16), "0xff");

    testSpelling(Number( { "123" }, { "u8" }, 10), "123u8");
    testSpelling(Number( { "123" }, { "u1" }, 10), "123u1");
    testSpelling(Number( { "123" }, { "a", "b", "c" }, 10), "123a'b'c"); */

    /* Console::printl("[SECTION] Integers - Tokenisation");

    testTokens("123", { number(false, { "123" }, {}, 10) });
    testTokens("010", { number(false, { "10" }, {}, 8) });
    testTokens("0b10", { number(false, { "10" }, {}, 2) });

    // Hexadecimal numbers are always lowercase internally.
    testTokens("0xff", { number(false, { "ff" }, {}, 16) });
    testTokens("0xfF", { number(false, { "ff" }, {}, 16) });
    testTokens("0xFf", { number(false, { "ff" }, {}, 16) });
    testTokens("0xFF", { number(false, { "ff" }, {}, 16) }); */

    /*testTokens("123u8", { Number( { "123" }, { "u8" }, 10) });
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

    /* Console::printl("[SECTION] Decimals - Spelling");

    testSpelling(Number( { "123", "912" }, {}, 10), "123.912");
    testSpelling(Number( { "123", "912", "0", "1000" }, {}, 10), "123.912.0.1000");
    testSpelling(Number( { "123", "912", "0" }, { "u8" }, 10), "123.912.0u8");
    testSpelling(Number( { "123", "0" }, { "u1" }, 10), "123.0u1");
    testSpelling(Number( { "123", "661" }, { "a", "b", "c" }, 10), "123.661a'b'c"); */

    /* Console::printl("[SECTION] Decimals - Tokenisation");

    testTokens("123", { Number( { "123" }, {}, 10) });
    testTokens("123u8", { Number( { "123" }, { "u8" }, 10) });
    testTokens("123u1", { Number( { "123" }, { "u1" }, 10) });
    testTokens("123a'b'c", { Number( { "123" }, { "a", "b", "c" }, 10) }); */
}
// Tests spelling for the specific variation of the token.
static bool checkSpelling(
    std::string_view expected,
    Token::VariantType const& innerToken,
    bool hasSpaceAfter
) {
    // The size for the final expected string.
    std::size_t _size = expected.size();
    // Reserves space for the space after the token.
    if (hasSpaceAfter) {
        _size += 1;
    }
    // The full expected string, large enough 
    std::string _expected = std::string(_size, '#');
    // Copies the core expected string to the full expected string.
    std::copy(expected.begin(), expected.end(), _expected.begin());
    // Sets the space (it could have been set during the construction of the string, but then 
    // it would be harder to see an error while copying the initial expected string).
    if (hasSpaceAfter) {
        *_expected.rbegin() = ' ';
    }
    // Constructs the token variation temporarily and gets its spelling.
    std::string _actual = Token(innerToken, hasSpaceAfter).GetSpelling();
    if (_actual != _expected) {
        // Prints the zero-based test index.
        Console::printl("[FAIL] Test {}", testCount - 1);
        Console::printl("[INPUT] hasSpaceAfter = {}", hasSpaceAfter? "true" : "false");
        Console::printl("[ACTUAL] {}", Text::quote(_actual));
        Console::printl("[EXPECTED] {}", Text::quote(_expected));
        return false;
    }
    return true;
}
void testSpelling(Token::VariantType const& innerToken, std::string_view expected) {
    ++testCount;
    if (
        // Without a space after.
        !checkSpelling(expected, innerToken, false)
        // With a space after.
        || !checkSpelling(expected, innerToken, true)
    ) {
        // Even if multiple variations fail, it still counts as a single error.
        ++failCount;
    }
}
void testTokens(std::string const& input, std::initializer_list<Token> const& expected) {
    ++testCount;

    try {
        std::deque<Token> const _actual = tokenise(input);

        auto _mismatch = std::mismatch(
            _actual.begin(), _actual.end(),
            expected.begin(), expected.end()
        );

        // using ItType = decltype(_mismatch.first);

        auto _listElems = [](auto _begin, auto _end)->std::string {
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