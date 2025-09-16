#include <fennton/grafik/Shader.hpp>
#include <fennton/grafik/Exceptions.hpp>

#include <glad/glad.h>

namespace Fennton::Grafik {
    Stage Stage::build(Stage::Type type, std::string_view src) {
        Stage _stage;
        _stage.Create(type);
        _stage.SetSource(src);
        // _stage.Compile();
        return _stage;
    }
    void Stage::Create(Stage::Type type) {
        std::uint32_t _type;
        switch (type) {
            case Type::Geometry:
                _type = GL_GEOMETRY_SHADER;
                break;
            case Type::Vertex:
                _type = GL_VERTEX_SHADER;
                break;
            case Type::Fragment:
                _type = GL_FRAGMENT_SHADER;
                break;
            /* case Type::Compute:
                _type = GL_COMPUTE_SHADER;
                break; */
            default:
                throw CreationException("Unknown shader type.");
        }
        this->type = type;
        this->id = glCreateShader(_type);
    }
    void Stage::SetSource(std::string_view src) {
        std::int32_t _size = src.size();
        char const* _data = src.data();
        glShaderSource(id, 1, &_data, &_size);
    }
}