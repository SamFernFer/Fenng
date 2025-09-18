#include <filesystem>
#include <string>
#include <stdexcept>

namespace Fennton::Filesystem {
    class Exception : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };
    // Retrieves the path to the current executable as a string.
    std::string getExecutablePathStr();
    // Retrieves the path to the current executable as a path.
    std::filesystem::path getExecutablePath();
}