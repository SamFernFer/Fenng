/*

*/

#include <fennton/grafik/Window.hpp>
#include <fennton/grafik/Shader.hpp>
#include <fennton/grafik/Exceptions.hpp>
#include <fennton/grafik/Time.hpp>
#include <fennton/utils/Console.hpp>
#include <fennton/utils/Text.hpp>
#include <fennton/utils/Filesystem.hpp>

#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glad/glad.h>
#include <stb_image.h>

#include <algorithm>
#include <exception>
#include <format>
#include <vector>
#include <optional>
#include <type_traits>
#include <filesystem>
#include <cstdint>

namespace fs = std::filesystem;
namespace Console = Fennton::Console;
namespace Text = Fennton::Text;
namespace Grafik = Fennton::Grafik;
namespace Fs = Fennton::Filesystem;

using namespace Fennton::Memory;

using Grafik::Window;
using Grafik::Monitor;
using Grafik::Stage;
using Grafik::Shader;
using Grafik::CompilationException;
using Grafik::LinkingException;
using Grafik::UniformException;

// Mesh using an element buffer object instead of only an array buffer.
struct Mesh {
    std::vector<std::uint32_t> indices = {};
    std::vector<float> vertices = {};
    // Stores the triangle count, so that there's no need to divide the size of the vertices 
    // vector by 3 with each draw call without an EBO.
    std::uint64_t triCount = 0;
    std::uint32_t vao = 0, vbo = 0, ebo = 0;
};
struct Texture {
    std::uint32_t id;
};

Mesh rectMesh;
Texture rectTex;
Stage vertShader, fragShader;
Shader rectProg;

Strong<Window> mainWindow = nullptr;

void init();
void term();
void loop();

// Gets the absolute path to the relative path inside the resources folder.
static fs::path getResPath(fs::path const& resPath);

// Creates a mesh object. If indices is empty, an element buffer object is not generated.
Mesh createMesh(
    std::vector<float> const& vertices, std::vector<uint32_t> const& indices
);
// Deletes the mesh from the GPU and empties clears its data in the CPU.
void deleteMesh(Mesh const& mesh);
void drawMesh(Mesh const& mesh);

Texture createTexture(fs::path const& path);
void deleteTexture(Texture const& texture);
// Binds the texture to the specified slot.
void bindTexture(Texture const& texture, std::uint32_t slot);

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
    mainWindow = Window::create(800, 600, "Texture", nullptr, nullptr);
    // A context is necessary before Grafik can be initialised.
    mainWindow->MakeContextCurrent();
    // Sets the swap interval to 1.
    Grafik::setSwapInterval(1);
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
        // Positions         // Colours.          // Texture coordinates.
         0.5f,  0.5f, 0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f,  1.0f,  1.0f,  0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f  // top left 
    };

    // The rectangle's vertex indices.
    std::vector<std::uint32_t> _indices = {
        0, 1, 3,   // First triangle
        1, 2, 3    // Second triangle
    };
    #endif

    rectMesh = createMesh(_verts, _indices);
    rectTex = createTexture(getResPath("textures/Art1(256x256).jpg"));
    
    vertShader = Stage::buildFromFile(Stage::Vertex, getResPath("shaders/Rect.vert"));
    fragShader = Stage::buildFromFile(Stage::Fragment, getResPath("shaders/Texture.frag"));
    rectProg = Shader::create();
    rectProg.Attach(vertShader);
    rectProg.Attach(fragShader);
    rectProg.Link();

    rectProg.Use();
    // Binds the texture.
    bindTexture(rectTex, 0);
    // Sets the uniforms.
    rectProg.Set("lightIntensity", 1.0f);
    constexpr float _lightRadius = 0.2f;
    rectProg.Set("lightSqrRadius", _lightRadius * _lightRadius);

    // rectProg.Set("tex1", 0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void term() {
    fragShader.Destroy();
    vertShader.Destroy();
    rectProg.Destroy();
    deleteTexture(rectTex);
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

            rectProg.Use();

            float _time = static_cast<float>(Grafik::getTime());

            constexpr float _interval = 2.0f;

            // Moves the light in a circle.
            float const _factor =
                (std::fmod(_time, _interval)
                * 2.0f / _interval - 1.0f) * glm::pi<float>()
            ;
            float const _x = std::cos(_factor), _y = std::sin(_factor);

            rectProg.TrySet("lightPos", glm::vec2(0.3f * _x, 0.3f * _y));

            drawMesh(rectMesh);
        }

        mainWindow->SwapBuffers();
    }
}
static fs::path getResPath(fs::path const& resPath) {
    return Fs::getExecutablePath()
        .parent_path()
        .parent_path()
        .parent_path()
        / "resources" / resPath
    ;
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

    // Positions.
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
        reinterpret_cast<void*>(0)
    );
    glEnableVertexAttribArray(0);

    // Colours.
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
        reinterpret_cast<void*>(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // Texture coordinates.
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
        reinterpret_cast<void*>(6 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

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

Texture createTexture(fs::path const& path) {
    std::string const _pathStr = path.generic_string();

    std::int32_t _width, _height, _channels;
    std::uint8_t* _texData = stbi_load(
        _pathStr.c_str(), &_width, &_height, &_channels, 0
    );
    if (!_texData) {
        throw Grafik::Exception(std::format(
            "Failed to load texture from path {}",
            Text::quote(_pathStr)
        ));
    }

    Texture _tex;
    glGenTextures(1, &_tex.id);
    glBindTexture(GL_TEXTURE_2D, _tex.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0, GL_RGB, _width, _height,
        0, GL_RGB, GL_UNSIGNED_BYTE,
        _texData
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(_texData);

    return _tex;
}
void deleteTexture(Texture const& texture) {
    glDeleteTextures(1, &texture.id);
}
void bindTexture(Texture const& texture, std::uint32_t slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture.id);
}