#include <fennton/utils/Text.hpp>
#include <fennton/utils/Console.hpp>

#include <cstdint>

using namespace Fennton;

static std::int64_t testCount = 0, failCount = 0;

template <bool alwaysShow> void testCase(std::string const& expected, std::string const& input) {
    ++testCount;
    bool _fail = false;
    {
        const std::string _actual = Text::escape(input);
        if (expected != _actual) {
            Console::printl(
                "[INPUT]    {0}\n"
                "[EXPECTED] {1}\n"
                "[ACTUAL]   {2}\n"
                "[FAIL] Test {3} (escape)",
                input, expected, _actual, testCount
            );
            _fail = true;
        }
    }
    {
        std::string _quoted = "\"" + std::string(expected) + "\"";
        const std::string _actual = Text::quote(input);
        if (_quoted != _actual) {
            Console::printl(
                "[INPUT]    {0}\n"
                "[EXPECTED] {1}\n"
                "[ACTUAL]   {2}\n"
                "[FAIL] Test {3} (quote)",
                input, _quoted, _actual, testCount
            );
            _fail = true;
        }
    }
    if (_fail) {
        ++failCount;
    }
}
template <bool alwaysShow> int test() {
    constexpr auto _case = testCase<alwaysShow>;
    _case("Hello, world!", "Hello, world!");
    _case(R"(first\nsecond\nthird)", "first\nsecond\nthird");
    
    #include <fennton/utils/TextQuoteGenerated.hpp>

    Console::printl(
        "[RESULT] {0}\n"
        "Failed: {1}/{2}",
        failCount == 0? "PASS" : "FAIL",
        failCount, testCount
    );
    return failCount != 0;
}
int main(int argc, char** argv) {
    if (argc > 1) {
        if (std::strcmp(argv[1], "manual") == 0) {
            return test<true>();
        } else if (std::strcmp(argv[1], "auto") == 0) {
            return test<false>();
        } else {
            std::cout << "Unknown mode \"" << argv[1] << "\"." << std::endl;
            return 0;
        }
    }
    return test<false>();
}