/*
    Tests rendering a multicoloured rectangle with a 2D point light over it. The aspect ratio 
    is not corrected based on the window's dimensions, so the light looks stretched at the 
    800x600 resolution. The drawing function now takes the triangle and element counts from the 
    Mesh struct and the triangle count is cached to save on having to perform the same division 
    with every draw call. Uses vertex colours, whose offsets are still hardcoded in the mesh 
    loading function.
*/

#include <fennton/grafik/Window.hpp>
#include <fennton/grafik/Exceptions.hpp>
#include <fennton/utils/Console.hpp>
#include <fennton/utils/Text.hpp>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <exception>
#include <format>
#include <vector>
#include <optional>
#include <type_traits>
#include <cstdint>

namespace Console = Fennton::Console;
namespace Text = Fennton::Text;
namespace Grafik = Fennton::Grafik;

using namespace Fennton::Memory;

using Grafik::Window;
using Grafik::Monitor;
using Grafik::CompilationException;
using Grafik::LinkingException;
using Grafik::UniformException;

enum class ShaderType {
    Vertex,
    Fragment
};

// Mesh using an element buffer object instead of only an array buffer.
struct Mesh {
    std::vector<std::uint32_t> indices = {};
    std::vector<float> vertices = {};
    // Stores the triangle count, so that there's no need to divide the size of the vertices 
    // vector by 3 with each draw call without an EBO.
    std::uint64_t triCount = 0;
    std::uint32_t vao = 0, vbo = 0, ebo = 0;
};

Mesh rectMesh;
std::uint32_t vertShader, fragShader, rectProg;

Strong<Window> mainWindow = nullptr;

void init();
void term();
void loop();

// Creates a mesh object. If indices is empty, an element buffer object is not generated.
Mesh createMesh(
    std::vector<float> const& vertices, std::vector<uint32_t> const& indices
);
// Deletes the mesh from the GPU and empties clears its data in the CPU.
void deleteMesh(Mesh const& mesh);
void drawMesh(Mesh const& mesh);
std::uint32_t createShader(ShaderType type, char const* src);
void deleteShader(std::uint32_t shader);
std::uint32_t createProgram();
void deleteProgram(std::uint32_t program);
void attachShader(std::uint32_t program, std::uint32_t shader);
void linkProgram(std::uint32_t program);
void useProgram(std::uint32_t program);
// Sets the float uniform, throwing if it could not be found.
void setUniform(std::uint32_t program, std::string const& name, float value);
// Sets the vec2 uniform, throwing if it could not be found.
void setUniform(std::uint32_t program, std::string const& name, glm::vec2 value);
// Sets the vec3 uniform, throwing if it could not be found.
void setUniform(std::uint32_t program, std::string const& name, glm::vec3 value);

// Sets the float uniform, doing nothing it could not be found.
void trySetUniform(std::uint32_t program, std::string const& name, float value);
// Sets the vec2 uniform, doing nothing it could not be found.
void trySetUniform(std::uint32_t program, std::string const& name, glm::vec2 value);
// Sets the vec3 uniform, doing nothing it could not be found.
void trySetUniform(std::uint32_t program, std::string const& name, glm::vec3 value);

int main() {
    try {
        init();
        loop();
    } catch (std::exception& e) {
        Console::printl("[EXCEPTION] {}", e.what());
        // Pauses to make it possible to see the exception.
        Console::pause();
    } catch (...) {
        Console::printl("[UNKNOWN EXCEPTION]");
        // Pauses to make it possible to see the exception.
        Console::pause();
    }
    term();
}

void init() {
    Console::init();
    Window::init();
    Monitor::init();

    // Creates the window.
    mainWindow = Window::create(800, 600, "Uniform", nullptr, nullptr);
    // A context is necessary before Grafik can be initialised.
    mainWindow->MakeContextCurrent();
    // Initialises the graphics module.
    Grafik::init();

    #if false
    // The rectangle's vertices, for using without an element buffer object.
    std::vector<float> _verts = {
        // first triangle
         0.5f,  0.5f, 0.0f,  0.0f,  0.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,  0.0f,  1.0f, 0.0f, // bottom right
        -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, // top left 
        // second triangle
         0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 1.0f, // bottom left
        -0.5f,  0.5f, 0.0f,  1.0f,  1.0f, 0.0f // top left
    };
    #else
    // The rectangle's vertex positions and colours, to be used with indices.
    std::vector<float> _verts = {
        // Positions         // Colours.
         0.5f,  0.5f, 0.0f,  0.0f,  0.0f,  1.0f, // top right
         0.5f, -0.5f, 0.0f,  0.0f,  1.0f,  0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f,  1.0f,  1.0f, // bottom left
        -0.5f,  0.5f, 0.0f,  1.0f,  0.0f,  0.0f // top left 
    };

    // The rectangle's vertex indices.
    std::vector<std::uint32_t> _indices = {
        0, 1, 3,   // First triangle
        1, 2, 3    // Second triangle
    };
    #endif

    rectMesh = createMesh(_verts, _indices);

    char const* _vertSrc = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColour;

        out vec3 vertexPos;
        out vec3 vertexColour;

        void main() {
            gl_Position = vec4(aPos, 1.0);
            vertexPos = aPos;
            vertexColour = aColour;
        }
    )";
    char const* _fragSrc = R"(
        #version 330 core
        out vec4 FragColour;
        
        in vec3 vertexPos;
        in vec3 vertexColour;

        uniform vec2 lightPos;
        uniform float lightIntensity;
        uniform float lightRadius;

        void main() {
            vec3 _col = vec3(0.75, 0.5, 0.25) * vertexColour;
            vec2 _delta = lightPos - vec2(vertexPos);
            // Linear attenuation.
            float _att = lightRadius - dot(_delta, _delta);
            // Divides by the radius to normalise it.
            _att /= lightRadius;
            // Calculates the lighting.
            _col *= _att * lightIntensity;
            // Outputs the fragment's colour.
            FragColour = vec4(_col, 1.0);
        }
    )";

    vertShader = createShader(ShaderType::Vertex, _vertSrc);
    fragShader = createShader(ShaderType::Fragment, _fragSrc);
    rectProg = createProgram();
    attachShader(rectProg, vertShader);
    attachShader(rectProg, fragShader);
    linkProgram(rectProg);

    useProgram(rectProg);
    // Sets the uniforms.
    trySetUniform(rectProg, "lightIntensity", 1.0f);
    trySetUniform(rectProg, "lightRadius", 0.2f);
    trySetUniform(rectProg, "lightPos", glm::vec2(0.0f, 0.1f));

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void term() {
    deleteShader(fragShader);
    deleteShader(vertShader);
    deleteMesh(rectMesh);

    mainWindow->Destroy();

    Monitor::term();
    Window::term();
    Console::term();
}
void loop() {
    while (!mainWindow->ShouldClose()) {
        Window::pollEvents();

        if (!mainWindow->IsIconified()) {
            glClearColor(0.1f, 0.3f, 0.15f, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        useProgram(rectProg);

        drawMesh(rectMesh);

        mainWindow->SwapBuffers();
    }
}

Mesh createMesh(
    std::vector<float> const& vertices, std::vector<uint32_t> const& indices
) {
    Mesh _mesh;
    _mesh.vertices = vertices;
    _mesh.triCount = vertices.size()/3;
    _mesh.indices = indices;

    glGenVertexArrays(1, &_mesh.vao);
    glBindVertexArray(_mesh.vao);

    glGenBuffers(1, &_mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _mesh.vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(std::remove_cvref_t<decltype(vertices)>::value_type),
        vertices.data(),
        GL_STATIC_DRAW
    );

    // If `indices` is not empty, generates an EBO to be used for drawing.
    if (!indices.empty()) {
        glGenBuffers(1, &_mesh.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh.ebo);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(std::remove_cvref_t<decltype(indices)>::value_type),
            indices.data(),
            GL_STATIC_DRAW
        );
    }

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
        reinterpret_cast<void*>(0)
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
        reinterpret_cast<void*>(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    return _mesh;
}
void deleteMesh(Mesh const& mesh) {
    // Deletes the VAO first, because else the EBO and VBO will unbind from it when they get 
    // deleted and then it would be probably slightly slower.
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(1, &mesh.ebo);
    glDeleteBuffers(1, &mesh.vbo);
}
void drawMesh(Mesh const& mesh) {
    glBindVertexArray(mesh.vao);
    if (mesh.ebo != 0) {
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, NULL);
    } else {
        // The size is divided by 3, because each position has 3 elements.
        glDrawArrays(GL_TRIANGLES, 0, mesh.triCount);
    }
}
std::uint32_t createShader(ShaderType type, char const* src) {
    std::uint32_t _type;
    switch (type) {
        case ShaderType::Vertex:
            _type = GL_VERTEX_SHADER;
            break;
        case ShaderType::Fragment:
            _type = GL_FRAGMENT_SHADER;
            break;
        default:
            throw CompilationException("Unknown shader type.");
    }
    
    std::uint32_t _shader = glCreateShader(_type);
    glShaderSource(_shader, 1, &src, NULL);
    glCompileShader(_shader);

    // NOTE: includes the null terminator.
    std::int32_t _logLength;
    glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &_logLength);

    std::string _infoLog = std::string(_logLength == 0? 0 : _logLength-1, '#');
    glGetShaderInfoLog(_shader, _logLength, NULL, _infoLog.data());

    std::string _msg = std::format("Grafik: {}", _infoLog);

    // Retrieves the compilation status for the shader.
    std::int32_t _succ;
    glGetShaderiv(_shader, GL_COMPILE_STATUS, &_succ);

    // Compilation was successful, so just prints what is in the log (maybe warnings).
    if (_succ == GL_TRUE) {
        if (_logLength != 0) {
            Console::printl(_msg);
        }
    } else {
        // Throws an exception because compilation failed.
        throw CompilationException(_msg);
    }
    return _shader;
}
void deleteShader(std::uint32_t shader) {
    glDeleteShader(shader);
}
std::uint32_t createProgram() {
    return glCreateProgram();
}
void deleteProgram(std::uint32_t program) {
    glDeleteProgram(program);
}
void attachShader(std::uint32_t program, std::uint32_t shader) {
    glAttachShader(program, shader);
}
void linkProgram(std::uint32_t program) {
    glLinkProgram(program);

    // NOTE: includes the null terminator.
    std::int32_t _logLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &_logLength);

    std::string _infoLog = std::string(_logLength == 0? 0 : _logLength-1, '#');

    // Retrieves the link status.
    std::int32_t _succ;
    glGetProgramiv(program, GL_LINK_STATUS, &_succ);

    std::string _msg = std::format("Grafik: {}", _infoLog);

    // If linking was successful, just prints any linking log which might exist.
    if (_succ == GL_TRUE) {
        if (_logLength != 0) {
            Console::printl(_msg);
        }
    } else {
        // Throws if linking failed.
        throw LinkingException(_msg);
    }
}
void useProgram(std::uint32_t program) {
    glUseProgram(program);
}
static std::int32_t getUniformOrThrow(std::uint32_t program, std::string const& name) {
    std::int32_t _loc = glGetUniformLocation(program, name.c_str());
    if (_loc < 0) {
        throw UniformException(std::format(
            "Uniform {} could not be found.", Text::quote(name)
        ));
    }
    return _loc;
}
void setUniform(std::uint32_t program, std::string const& name, float value) {
    glUniform1f(getUniformOrThrow(program, name), value);
}
void setUniform(std::uint32_t program, std::string const& name, glm::vec2 value) {
    glUniform2f(getUniformOrThrow(program, name), value.x, value.y);
}
void setUniform(std::uint32_t program, std::string const& name, glm::vec3 value) {
    glUniform3f(getUniformOrThrow(program, name), value.x, value.y, value.z);
}

void trySetUniform(std::uint32_t program, std::string const& name, float value) {
    std::int32_t _loc = glGetUniformLocation(program, name.c_str());
    if (_loc < 0) {
        return;
    }
    glUniform1f(getUniformOrThrow(program, name), value);
}
void trySetUniform(std::uint32_t program, std::string const& name, glm::vec2 value) {
    std::int32_t _loc = glGetUniformLocation(program, name.c_str());
    if (_loc < 0) {
        return;
    }
    glUniform2f(getUniformOrThrow(program, name), value.x, value.y);
}
void trySetUniform(std::uint32_t program, std::string const& name, glm::vec3 value) {
    std::int32_t _loc = glGetUniformLocation(program, name.c_str());
    if (_loc < 0) {
        return;
    }
    glUniform3f(getUniformOrThrow(program, name), value.x, value.y, value.z);
}