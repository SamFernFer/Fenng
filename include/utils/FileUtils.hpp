#pragma once
#ifndef FENNTON_FILEUTILS_HPP
#define FENNTON_FILEUTILS_HPP
#include <cstdint>
#include <string>

namespace Fennton::FileUtils
{
    // Returns the path to the enclosing executable.
    std::string getExecutablePath();
    // Returns the path to the enclosing module.
    std::string getModulePath();
};
#endif