#ifndef FENNTON_RESOURCES_HPP
#define FENNTON_RESOURCES_HPP

#include <filesystem>
#include <cstdint>

namespace Fennton::Resources {
    // Initialises the paths.
    void init();
    // Gets the executable's path.
    std::filesystem::path getExePath();
    // Gets the root path.
    std::filesystem::path getRootPath();
    // Gets a path inside the "resources" directory.
    std::filesystem::path getPath(std::filesystem::path const& path);
    // Gets the path to the "resources" directory.
    std::filesystem::path getPath();
    // Initialises the quad mesh, mostly used for rendering to the screen with a shader (like for 
    // post-processing effects).
    void initQuad();
    // Returns the VAO for a the fullscreen quad mesh.
    std::uint32_t getQuadVAO();
}

#endif