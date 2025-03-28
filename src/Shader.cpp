#include <Shader.hpp>
#include <glad/glad.h>
#include <vector>
#include <stdexcept>

namespace Fennton {
    ShaderKind Shader::GetKind() const {
        return kind;
    }
    std::uint32_t Shader::GetID() const {
        return id;
    }
    Shader::Shader(const char* code, ShaderKind kind) : kind(kind) {
        std::uint32_t _kindEnum;
        switch (kind) {
            case ShaderKind::Vertex:
                _kindEnum = GL_VERTEX_SHADER;
                break;
            case ShaderKind::Fragment:
                _kindEnum = GL_FRAGMENT_SHADER;
                break;
        }
        // Creates the shader.
        id = glCreateShader(_kindEnum);
        // Sets the shader's source.
        glShaderSource(id, 1, &code, NULL);
        // Compiles the shader.
        glCompileShader(id);
        // The variable to store the compilation status.
        std::int32_t _succ;
        // Checks if there has been a compilation error.
        glGetShaderiv(id, GL_COMPILE_STATUS, &_succ);
        // If compilation was unsuccessful.
        if (!_succ) {
            std::int32_t _length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &_length);
            // Initialises a vector with the length of the info log.
            std::vector<char> _log(_length);
            // Copies the info log into the vector.
            glGetShaderInfoLog(id, _length, NULL, _log.data());
            // Makes sure the shader is deleted.
            Shader::~Shader();
            // Throws an exception with information about what went wrong.
            throw std::runtime_error(
                "[Shader Compilation Failed]\n" + std::string(_log.data())
            );
        }
    }
    Shader::~Shader() {
        // Deletes the shader.
        glDeleteShader(id);
    }

    std::uint32_t Program::GetID() const {
        return id;
    }
    Program::Program(const char* vert, const char* frag) {
        Shader _vert = Shader(vert, ShaderKind::Vertex);
        Shader _frag = Shader(frag, ShaderKind::Fragment);

        id = glCreateProgram();
        glAttachShader(id, _vert.GetID());
        glAttachShader(id, _frag.GetID());
        glLinkProgram(id);

        // The variable to store the link status.
        std::int32_t _succ;
        // Checks if there has been a compilation error.
        glGetProgramiv(id, GL_LINK_STATUS, &_succ);
        // If compilation was unsuccessful.
        if (!_succ) {
            std::int32_t _length;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &_length);
            // Initialises a vector with the length of the info log.
            std::vector<char> _log(_length);
            // Copies the info log into the vector.
            glGetProgramInfoLog(id, _length, NULL, _log.data());
            // Makes sure the shader is deleted.
            Program::~Program();
            // Throws an exception with information about what went wrong.
            throw std::runtime_error(
                "[Shader Link Failed]\n" + std::string(_log.data())
            );
        }
    }
    Program::~Program() {
        // Deletes the program.
        glDeleteProgram(id);
    }
    void Program::Use() const {
        glUseProgram(id);
    }
}