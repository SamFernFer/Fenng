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
using Grafik::LinkingException;

std::uint32_t vbo;
std::uint32_t vertShader, fragShader, triProg;

Strong<Window> mainWindow = nullptr;

void init();
void term();
void loop();

std::uint32_t createMesh(std::uint64_t size, float* vertices);
std::uint32_t createShader(std::uint32_t type, char const* src);
void deleteShader(std::uint32_t shader);
std::uint32_t createProgram();
void deleteProgram(std::uint32_t program);
void attachShader(std::uint32_t program, std::uint32_t shader);
void linkProgram(std::uint32_t program);

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
        layout (location = 0) in vec3 aPos;
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
    triProg = createProgram();
    attachShader(triProg, vertShader);
    attachShader(triProg, fragShader);
    linkProgram(triProg);
}
void term() {
    deleteShader(fragShader);
    deleteShader(vertShader);

    mainWindow->Destroy();

    Console::pause();

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
    switch (type) {
        case GL_VERTEX_SHADER:
        case GL_FRAGMENT_SHADER:
            break;
        default:
            throw CompilationException("Unknown shader type.");
    }
    
    std::uint32_t _shader = glCreateShader(type);
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