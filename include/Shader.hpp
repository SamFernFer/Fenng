#ifndef FENNTON_SHADER_HPP
#define FENNTON_SHADER_HPP

#include <cstdint>

namespace Fennton {
    class Shader;
    class Program;
    // An OpenGL shader's kind. Used for constructing Shader objects.
    enum class ShaderKind {
        Vertex, Fragment
    };
    // A class representing an OpenGL shader.
    class Shader {
        ShaderKind kind;
        std::uint32_t id;
    public:
        ShaderKind GetKind() const;
        std::uint32_t GetID() const;
        Shader(const char* code, ShaderKind kind);
        ~Shader();
    };
    class Program {
        std::uint32_t id;
    public:
        std::uint32_t GetID() const;
        Program(const char* vert, const char* frag);
        ~Program();
        void Use() const;
    };
}
#endif