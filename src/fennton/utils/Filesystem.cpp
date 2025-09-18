#include <fennton/utils/Filesystem.hpp>
#include <whereami.h>
#include <cstdint>

namespace Fennton::Filesystem {
    std::string getExecutablePathStr() {
        std::int32_t const _length = wai_getExecutablePath(NULL, 0, NULL);
        std::string _pathStr = std::string(_length, '#');
        wai_getExecutablePath(_pathStr.data(), _length, NULL);
        char const _lastByte = _pathStr.data()[_length];
        // Makes sure the last byte is zero, in case something got copied over it.
        if (_lastByte != '\0') {
            throw Exception(std::format(
                "getExecutablePathStr: Null terminator got written over with byte {}.",
                static_cast<int>(_lastByte)
            ));
        }
        return std::move(_pathStr);
    }
    std::filesystem::path getExecutablePath() {
        return std::filesystem::path(getExecutablePathStr());
    }
}