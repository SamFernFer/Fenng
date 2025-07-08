#include <fennton/utils/Console.hpp>

#include <functional>
#include <sstream>
#include <string>
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <typeinfo>

static std::int64_t failCount = 0, testCount = 0;
Fennton::Console::Printer* printer = nullptr;

struct Class {
public:
    inline static std::int64_t instanceCount = 0;
    std::string name;
    Class(std::string const& name = "ERROR") {
        this->name = name;
        ++instanceCount;
    }

    friend std::ostream& operator<<(std::ostream& out, Class const& obj) {
        out << instanceCount << " instance(s): " << obj.name;
        return out;
    }
};

static std::string quote(std::string const& str) {
    return "\"" + str + "\"";
}

// Tests a specific case for the Console::print and Console::println functions.
template<typename... A> void testCase(
    std::string_view expected,
    A&&... args
    // std::function<bool()> const& extraCond = nullptr
) {
    // Registers the test.
    ++testCount;
    // This variable will be used to know whether to test the version with a line break at 
    // the end.
    bool _fail = false;
    {
        // Prints.
        if (printer) {
            if constexpr (requires {
                printer->print(args...);
            }) {
                printer->print(args...);
            }
        } else {
            if constexpr (requires {
                Fennton::Console::print(args...);
            }) {
                // The printing function without the new-line is tested alongside the one with 
                // the new-line.
                Fennton::Console::print(args...);
                Fennton::Console::print([]()->std::string{
                    std::stringstream _ss;
                    _ss << std::endl;
                    return _ss.str();
                }());
            }
        }

        if (printer) {
            std::string _type = typeid(printer->getStream()).name();
            std::stringstream& _ss = dynamic_cast<std::stringstream&>(printer->getStream());
            // Checks if the text is correct and the extra condition is met.
            std::string _str = _ss.str();
            if (_str != expected) {
                std::cout << "[EXPECTED] " << quote(std::string(expected)) << "" << std::endl;
                std::cout << "[ACTUAL]   " << quote(_str) << "" << std::endl;
                _fail = true;
            }
            if (_fail) {
                ++failCount;
                std::cout << "[FAIL] Test " << testCount << std::endl;
            }
            // Resets the stream.
            _ss.str("");
            _ss.clear();
        }
    }
    if (!printer) {
        Fennton::Console::printl();
        Fennton::Console::printl();
    }
    // Fail can only be set to true if the test fails or if there's no printer.
    if (!_fail) {
        // Prints.
        if (printer) {
            printer->printl(args...);
        } else {
            Fennton::Console::printl(args...);
        }

        if (printer) {
            std::stringstream& _ss = dynamic_cast<std::stringstream&>(printer->getStream());
            // The expected output, with an added line break at the end.
            std::string _expectedln = [expected](){
                std::stringstream _ss;
                _ss << std::string(expected) << std::endl;
                return _ss.str();
            }();
            if (_ss.str() != _expectedln) {
                std::cout << "[REASON] Possible missing line break." << std::endl;
            }
            // Resets the stream again.
            _ss.str("");
            _ss.clear();
        }
    }
}

template<bool manual> int test() {
    std::stringstream _ss;
    Fennton::Console::Printer _printerObj = Fennton::Console::Printer(_ss);

    if constexpr (!manual) {
        printer = &_printerObj;
    }

    // Testing printing a line break on its own.
    if constexpr (manual)
        Fennton::Console::pause("[TEST] Empty.");

    testCase("");

    // Testing Console::print and Testing Console::printl.
    if constexpr (manual)
        Fennton::Console::pause("[TEST] Single value.");

    std::string _str1 = "My string.";
    testCase(_str1, "My string.");

    testCase("", "");
    testCase("89", 89);
    testCase("0", 0);
    testCase("nullptr", nullptr);

    // Printing boolean values, by default, is weird.
    // testCase(true, "true");

    testCase("Hello, World!", "Hello, World!");
    testCase("Olá, mundo!", "Olá, mundo!");
    testCase("どうもありがとう", "どうもありがとう");
    testCase("c", 'c');
    testCase("\n", '\n');
    testCase("first\nsecond\nthird", "first\nsecond\nthird");
    testCase("8", 010);

    Class _obj1("named");
    testCase("1 instance(s): named", _obj1);

    Class _obj2;
    testCase("2 instance(s): ERROR", _obj2);

    // Formatting.
    if constexpr (manual)
        Fennton::Console::pause("[TEST] Formatting.");

    testCase("number: 89", "number: {}", 89);
    testCase("0", "{1}", 101, 0);
    testCase("8", "{0}", 010);
    testCase("This is 0x0", "This is {}", nullptr);

    if constexpr(manual) {
        Fennton::Console::print("[END]");
        Fennton::Console::pause();
        return 0;
    } else {
        printer = nullptr;

         // The failure count must be zero for the automatic test to pass.
        bool _pass = failCount == 0;
        // Print the amount of errors and the test's result.
        std::cout
            << "[FINAL] " << (_pass? "PASS" : "FAIL") << ": "
            << (testCount - failCount) << "/" << testCount
        << std::endl;

        // Zero means good as an error code, so the boolean is inverted.
        return !_pass;
    }
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