#include <fennton/grafik/Window.hpp>
#include <fennton/grafik/Exceptions.hpp>
#include <fennton/utils/Console.hpp>
#include <fennton/utils/Text.hpp>

#include <glad/glad.h>

#include <exception>
#include <format>
#include <cstdint>

namespace Console = Fennton::Console;
namespace Text = Fennton::Text;
namespace Grafik = Fennton::Grafik;

using namespace Fennton::Memory;

using Grafik::Window;
using Grafik::Monitor;
using Grafik::CompilationException;

std::uint32_t vbo;
std::uint32_t vertShader, fragShader;

Strong<Window> mainWindow = nullptr;

void init();
void term();
void loop();

std::uint32_t createMesh(std::uint64_t size, float* vertices);
std::uint32_t createShader(std::uint32_t type, char const* src);
void deleteShader(std::uint32_t shader);

int main() {
    try {
        init();
        loop();
    } catch (std::exception& e) {
        Console::printl("[EXCEPTION] {}", e.what());
    } catch (...) {
        Console::printl("[UNKNOWN EXCEPTION]");
    }
    term();
}

void init() {
    Console::init();
    Window::init();
    Monitor::init();

    // Creates the window.
    mainWindow = Window::create(800, 600, "Triangle", nullptr, nullptr);
    // A context is necessary before Grafik can be initialised.
    mainWindow->MakeContextCurrent();
    // Initialises the graphics module.
    Grafik::init();
    // The triangle's vertices.
    float _verts[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    vbo = createMesh(sizeof(_verts), _verts);

    char const* _vertSrc = R"(
        #version 330 core
        layout (location = 0) in vec3 aPous;
        void main() {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
    )";
    char const* _fragSrc = R"(
        #version 330 core
        out vec4 FragColour;

        void main() {
            FragColour = vec4(1.0, 0.0, 0.0, 1.0);
        }
    )";

    vertShader = createShader(GL_VERTEX_SHADER, _vertSrc);
    fragShader = createShader(GL_FRAGMENT_SHADER, _fragSrc);
}
void term() {
    deleteShader(fragShader);
    deleteShader(vertShader);

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
        mainWindow->SwapBuffers();
    }
}

std::uint32_t createMesh(std::uint64_t size, float* vertices) {
    std::uint32_t _vbo;
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    return _vbo;
}
std::uint32_t createShader(std::uint32_t type, char const* src) {
    std::uint32_t _shader = glCreateShader(type);
    glShaderSource(_shader, 1, &src, NULL);
    glCompileShader(_shader);

    // NOTE: includes the null terminator.
    std::int32_t _logLength;
    glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &_logLength);

    std::string _infoLog = std::string(_logLength-1, '#');
    glGetShaderInfoLog(_shader, _logLength, NULL, _infoLog.data());

    // Retrieves the compilation status for the shader.
    std::int32_t _succ;
    glGetShaderiv(_shader, GL_COMPILE_STATUS, &_succ);
    // Compilation was successful, so just prints what is in the log (maybe warnings).
    if (_succ == GL_TRUE) {
        switch (type) {
            case GL_VERTEX_SHADER:
            case GL_FRAGMENT_SHADER:
                Console::printl("Grafik: {}", _infoLog);
                break;
            default:
                throw CompilationException("Unknown shader type.");
        }
    } else {
        // Throws an exception because compilation failed.
        throw CompilationException(std::format("Grafik: {}", _infoLog));
    }
    return _shader;
}
void deleteShader(std::uint32_t shader) {
    glDeleteShader(shader);
}