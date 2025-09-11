#include <stdexcept>

namespace Fennton::Grafik {
    class Exception : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class CompilationException : public Exception {
    public:
        using Exception::Exception;
    };
}