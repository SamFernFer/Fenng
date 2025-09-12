#ifndef FENNTON_GRAFIK_EXCEPTIONS_HPP
#define FENNTON_GRAFIK_EXCEPTIONS_HPP

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

    class LinkingException : public Exception {
    public:
        using Exception::Exception;
    };
}
#endif