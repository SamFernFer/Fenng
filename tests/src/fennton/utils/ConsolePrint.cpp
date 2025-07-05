#include <fennton/utils/Console.hpp>

#include <functional>
#include <sstream>
#include <string>
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstring>

static std::int64_t failCount = 0, testCount = 0;

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

// Tests a specific case for the Console::print and Console::println functions.
template<typename T> void testCase(
    std::stringstream& stream, T const& v,
    std::string_view expected,
    std::function<bool()> const& extraCond = nullptr
) {
    // Registers the test.
    ++testCount;
    // This variable will be used to know whether to test the version with a line break at 
    // the end.
    bool _fail = false;
    {
        // Prints.
        Fennton::Console::print(stream, v);
        // Checks if the text is correct and the extra condition is met.
        std::string _str = stream.str();
        if (_str != expected) {
            std::cout << "[EXPECTED] \"" << expected << "\"" << std::endl;
            std::cout << "[ACTUAL]   \"" << _str << "\"" << std::endl;
            _fail = true;
        }
        if (extraCond != nullptr && !extraCond()) {
            _fail = true;
        }
        if (_fail) {
            ++failCount;
            std::cout << "[FAIL] Test " << testCount << std::endl;
        }
        // Resets the stream.
        stream.str("");
        stream.clear();
    }
    if (!_fail) {
        Fennton::Console::println(stream, v);

        // The expected output, with an added line break at the end.
        std::string _expectedln = [expected](){
            std::stringstream _ss;
            _ss << std::string(expected) << std::endl;
            return _ss.str();
        }();
        if (stream.str() != _expectedln) {
            std::cout << "[REASON] Possible missing line break." << std::endl;
        }
        // Resets the stream again.
        stream.str("");
        stream.clear();
    }
}

int manualTest() {
    std::cout << "Manual test not implemented yet." << std::endl;
    return -1;
}
int autoTest() {
    std::stringstream _ss;

    // Testing Console::print and Testing Console::println.

    std::string _str1 = "My string.";
    testCase(_ss, _str1, "My string.");

    testCase(_ss, "", "");
    testCase(_ss, 89, "89");
    testCase(_ss, 0, "0");
    testCase(_ss, nullptr, "nullptr");

    // Printing boolean values, by default, is weird.
    // testCase(_ss, true, "true");

    testCase(_ss, "Hello, World!", "Hello, World!");
    testCase(_ss, "Olá, mundo!", "Olá, mundo!");
    testCase(_ss, "どうもありがとう", "どうもありがとう");
    testCase(_ss, 'c', "c");
    testCase(_ss, 010, "8");

    auto instanceCondition = []()->bool {
        // Checks if the object passed to the function is not copied.
        bool _pass = Class::instanceCount == 1;
        if (!_pass) {
            std::cout << "[EXPECTED] Class::instanceCount == 1" << std::endl;
            std::cout <<
                "[ACTUAL]   Class::instanceCount == " << Class::instanceCount
            << std::endl;
        }
        return _pass;
    };

    Class _obj1("named");
    testCase(_ss, _obj1, "1 instance(s): named", instanceCondition);

    Class _obj2;
    testCase(_ss, _obj2, "2 instance(s): ERROR");

    // Testing Console::printf.


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

int main(int argc, char** argv) {
    if (argc > 1) {
        if (std::strcmp(argv[1], "manual") == 0) {
            return manualTest();
        } else if (std::strcmp(argv[1], "auto") == 0) {
            return autoTest();
        } else {
            std::cout << "Unknown mode \"" << argv[1] << "\"." << std::endl;
            return 0;
        }
    }
    return autoTest();
}