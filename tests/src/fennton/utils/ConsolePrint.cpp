#include <fennton/utils/Console.hpp>

#include <functional>
#include <sstream>
#include <string>
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstring>

static std::int64_t failCount = 0, testCount = 0;

template<typename T> void testCase(
    std::stringstream& stream, T const& v,
    std::string_view expected,
    std::function<bool()> const& extraCond = nullptr
) {
    // Registers the test.
    ++testCount;
    // Prints.
    Fennton::Console::print(stream, v);
    bool _fail = false;
    // Checks if the text is correct and the extra condition is met.
    std::string _str = stream.str();
    if (_str != expected) {
        std::cout << "[EXPECTED] " << expected << std::endl;
        std::cout << "[ACTUAL]   " << _str << std::endl;
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

int manualTest() {
    std::cout << "Manual test not implemented yet." << std::endl;
    return -1;
}
int autoTest() {
    std::stringstream _ss;

    testCase(_ss, 89, "89");
    testCase(_ss, "Hello World!", "Hello World!");
    testCase(_ss, 'c', "c");
    testCase(_ss, 010, "8");

    // _obj.instanceCount == 0;
    ;
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