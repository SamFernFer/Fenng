#include <fennton/grafik/Shader.hpp>
#include <fennton/grafik/Exceptions.hpp>
#include <fennton/utils/Text.hpp>

#include <glad/glad.h>

#include <format>
#include <sstream>
#include <fstream>
#include <utility>

namespace fs = std::filesystem;
namespace Fennton::Grafik {
    static std::string stringFromFile(std::filesystem::path const& path) {
        if (!fs::is_regular_file(path)) {
            throw Exception(std::format(
                "Not a file: {}", Text::quote(path.generic_string())
            ));
        }
        std::fstream _file;
        _file.exceptions(std::ios::failbit | std::ios::badbit);
        _file.open(path);
        std::stringstream _ss;
        _ss << _file.rdbuf();
        return _ss.str();
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
            throw CompilationException(infoLog);
        }
    }
    void Stage::MoveFrom(Stage&& other) {
        this->infoLog = std::move(other.infoLog);
        this->type = std::move(other.type);
        this->id = std::move(other.id);
        other.Reset();
    }
    void Stage::Reset() {
        infoLog = {};
        type = None;
        id = 0;
    }
    std::uint32_t Stage::GetID() const {
        return id;
    }
    Stage::Stage(Stage&& other) {
        MoveFrom(std::move(other));
    }
    Stage& Stage::operator=(Stage&& other) {
        MoveFrom(std::move(other));
        return *this;
    }
    Stage::~Stage() {
        Destroy();
    }
    Stage Stage::build(Stage::Type type, std::string_view src) {
        Stage _stage;
        _stage.Create(type);
        _stage.SetSource(src);
        _stage.Compile();
        return _stage;
    }
    Stage Stage::buildFromFile(Stage::Type type, std::filesystem::path const& path) {
        return Stage::build(type, stringFromFile(path));
    }
    void Stage::Destroy() {
        if (id != 0) {
            glDeleteShader(id);
        }
        // Resets all fields.
        Reset();
    }

    void Shader::MoveFrom(Shader&& other) {
        this->infoLog = std::move(other.infoLog);
        this->id = std::move(other.id);
        other.Reset();
    }
    void Shader::Reset() {
        infoLog = {};
        id = 0;
    }
    Shader::Shader(Shader&& other) {
        MoveFrom(std::move(other));
    }
    Shader& Shader::operator=(Shader&& other) {
        MoveFrom(std::move(other));
        return *this;
    }
    Shader::~Shader() {
        Destroy();
    }
    Shader Shader::create() {
        Shader _shader;
        _shader.id = glCreateProgram();
        return _shader;
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
        glLinkProgram(id);

        // NOTE: includes the null terminator.
        std::int32_t _logLength;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &_logLength);

        infoLog = std::string(_logLength == 0? 0 : _logLength-1, '#');
        // Writes the info log into the string, even if there are no errors, because it might 
        // still contain something.
        glGetProgramInfoLog(id, _logLength, NULL, infoLog.data());

        // Retrieves the link status.
        std::int32_t _succ;
        glGetProgramiv(id, GL_LINK_STATUS, &_succ);

        // If linking was successful, just prints any linking log which might exist.
        if (_succ == GL_FALSE) {
            // Throws if linking failed.
            throw LinkingException(infoLog);
        }
    }
    void Shader::Use() {
        glUseProgram(id);
    }
    // Retrieves the uniform location or throws on error.
    static std::int32_t getLoc(std::uint32_t program, std::string const& name) {
        std::int32_t _loc = glGetUniformLocation(program, name.c_str());
        if (_loc < 0) {
            throw UniformException(std::format(
                "Uniform {} could not be found.", Text::quote(name)
            ));
        }
        return _loc;
    }
    static std::int32_t tryGetLoc(std::uint32_t program, std::string const& name) {
        return glGetUniformLocation(program, name.c_str());
    }
    void Shader::Set(std::string const& name, float value) {
        glUniform1f(getLoc(id, name), value);
    }
    void Shader::Set(std::string const& name, glm::vec2 value) {
        glUniform2f(getLoc(id, name), value.x, value.y);
    }
    void Shader::Set(std::string const& name, glm::vec3 value) {
        glUniform3f(getLoc(id, name), value.x, value.y, value.z);
    }
    void Shader::Set(std::string const& name, glm::vec4 value) {
        glUniform4f(getLoc(id, name), value.x, value.y, value.z, value.w);
    }

    bool Shader::TrySet(std::string const& name, float value) {
        auto _loc = tryGetLoc(id, name);
        if (_loc < 0) { return false; }
        glUniform1f(getLoc(id, name), value);
        return true;
    }
    bool Shader::TrySet(std::string const& name, glm::vec2 value) {
        auto _loc = tryGetLoc(id, name);
        if (_loc < 0) { return false; }
        glUniform2f(getLoc(id, name), value.x, value.y);
        return true;
    }
    bool Shader::TrySet(std::string const& name, glm::vec3 value) {
        auto _loc = tryGetLoc(id, name);
        if (_loc < 0) { return false; }
        glUniform3f(getLoc(id, name), value.x, value.y, value.z);
        return true;
    }
    bool Shader::TrySet(std::string const& name, glm::vec4 value) {
        auto _loc = tryGetLoc(id, name);
        if (_loc < 0) { return false; }
        glUniform4f(getLoc(id, name), value.x, value.y, value.z, value.w);
        return true;
    }
}