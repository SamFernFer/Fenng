#include <fennton/grafik/Shader.hpp>
#include <fennton/grafik/Exceptions.hpp>
#include <fennton/utils/Text.hpp>

#include <glad/glad.h>

#include <format>
#include <sstream>
#include <fstream>

namespace fs = std::filesystem;
namespace Fennton::Grafik {
    /* static std::string stringFromFile(std::filesystem::path const& path) {
        if (!fs::is_file()) {
            throw Exception(std::format("", Text::quote(path.generic_string())));
        }
        std::fstream _file;
        _file.exceptions();
        std::stringstream _ss;
        return _ss.str();
    } */
    Stage::~Stage() {
        Destroy();
    }
    void Stage::Create(Stage::Type type) {
        std::uint32_t _rawType;
        // Sets the raw type based on the enum value passed.
        switch (type) {
            case Type::Geometry:
                _rawType = GL_GEOMETRY_SHADER;
                break;
            case Type::Vertex:
                _rawType = GL_VERTEX_SHADER;
                break;
            case Type::Fragment:
                _rawType = GL_FRAGMENT_SHADER;
                break;
            /* case Type::Compute:
                _rawType = GL_COMPUTE_SHADER;
                break; */
            default:
                throw CreationException("Unknown shader type.");
        }
        this->type = type;
        this->id = glCreateShader(_rawType);
    }
    void Stage::SetSource(std::string_view src) {
        std::int32_t _size = src.size();
        char const* _data = src.data();
        glShaderSource(id, 1, &_data, &_size);
    }
    void Stage::Compile() {
        glCompileShader(id);

        // NOTE: includes the null terminator.
        std::int32_t _logLength;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &_logLength);

        // Prepares the storage for the info log.
        infoLog = std::string(_logLength == 0? 0 : _logLength-1, '#');
        // Writes the info log into the string, even if there are no errors, because it might 
        // still contain something.
        glGetShaderInfoLog(id, _logLength, NULL, infoLog.data());

        // Retrieves the compilation status for the shader.
        std::int32_t _succ;
        glGetShaderiv(id, GL_COMPILE_STATUS, &_succ);

        if (_succ == GL_FALSE) {
            // Throws an exception because compilation failed.
            throw CompilationException(std::format("Grafik: {}", infoLog));
        }
    }
    void Stage::Reset() {
        infoLog = {};
        type = None;
        id = 0;
    }
    std::uint32_t Stage::GetID() const {
        return id;
    }

    Stage Stage::build(Stage::Type type, std::string_view src) {
        Stage _stage;
        _stage.Create(type);
        _stage.SetSource(src);
        _stage.Compile();
        return _stage;
    }
    /* Stage Stage::buildFromFile(Stage::Type type, std::filesystem::path const& path) {
        return Stage::build(type, stringFromFile(path));
    } */
    void Stage::Destroy() {
        if (id != 0) {
            glDeleteShader(id);
        }
        // Resets all fields.
        Reset();
    }

    void Shader::Reset() {
        id = 0;
    }
    Shader::~Shader() {
        Destroy();
    }
    Shader Shader::create() {
        Shader _shader;
        _shader.id = glCreateProgram();
    }
    void Shader::Destroy() {
        if (id != 0) {
            glDeleteProgram(id);
        }
        Reset();
    }
    void Shader::Attach(Stage const& stage) {
        glAttachShader(id, stage.GetID());
    }
    void Shader::Link() {
        glLinkProgram(program);

        // NOTE: includes the null terminator.
        std::int32_t _logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &_logLength);

        infoLog = std::string(_logLength == 0? 0 : _logLength-1, '#');

        // Retrieves the link status.
        std::int32_t _succ;
        glGetProgramiv(program, GL_LINK_STATUS, &_succ);

        std::string _msg = ;

        // If linking was successful, just prints any linking log which might exist.
        if (_succ == GL_TRUE) {
            if (_logLength != 0) {
                Console::printl(_msg);
            }
        } else {
            // Throws if linking failed.
            throw LinkingException(std::format("Grafik: {}", infoLog));
        }
    }
}