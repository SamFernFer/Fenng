#include <glm/glm.hpp>

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
        Type type = None;
        std::uint32_t id = 0;
    public:
        // Constructs a null shader stage.
        Stage() = default;
        Stage(Stage const&) = delete;
        Stage(Stage&&) = default;
        Stage& operator=(Stage const&) = delete;
        Stage& operator=(Stage&&) = default;
        // Creates a shader stage of the specified type and with the specified source code 
        // and compiles it.
        static Stage build(Type type, std::string_view src);
        // Creates the internal shader object with the specified type.
        void Create(Type type);
        // Sets or replaces the shader stage's source code.
        void SetSource(std::string_view src);
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