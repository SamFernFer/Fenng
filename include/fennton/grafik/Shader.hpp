#include <string>
#include <cstdint>

namespace Fennton::Grafik {
    enum class ShaderType {
        Vertex,
        Fragment
    };
    // An OpenGL shader stage.
    class Stage {
    public:
        enum class Type {
            None, Geometry, Vertex, Fragment, Compute
        };
        using enum Type;
    private:
        std::uint32_t id = 0;
    public:
        // Creates a shader stage of the specified type and with the specified source code.
        static Stage create(Type type, std::string_view src);
        // Constructs a null shader stage.
        Stage() = default;
        Stage(Stage const&) = delete;
        Stage(Stage&&) = default;
        Stage& operator=(Stage const&) = delete;
        Stage& operator=(Stage&&) = default;
    };
    // An OpenGL shader program.
    class Shader {
    private:
        std::uint32_t id = 0;
    public:
        Shader() = default;
        Shader(Shader const&) = delete;
        Shader(Shader&&) = default;
        Shader& operator=(Shader const&) = delete;
        Shader& operator=(Shader&&) = default;
    };
}