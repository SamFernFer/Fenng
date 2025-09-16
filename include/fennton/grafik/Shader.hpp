#include <cstdint>

namespace Fennton::Grafik {
    enum class ShaderType {
        Vertex,
        Fragment
    };
    // An OpenGL shader stage.
    class Stage {
        enum class Type {
            Geometry, Vertex, Fragment, Compute
        };
    private:
        std::uint32_t id = 0;
    public:
        Stage();
        Stage(Stage const&) = delete;
        Stage(Stage&&) = default;
        operator=(Stage const&) = delete;
        operator=(Stage&&) = default;
    };
    // An OpenGL shader program.
    class Shader {
    private:
        std::uint32_t id = 0;
    public:
        Shader();
        Shader(Shader const&) = delete;
        Shader(Shader&&) = default;
        operator=(Shader const&) = delete;
        operator=(Shader&&) = default;
    };
}