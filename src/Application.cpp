#include <Application.hpp>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <Resources.hpp>
#include <Texture.hpp>
#include <Shader.hpp>
#include <Settings.hpp>
#include <Window.hpp>
#include <Memory.hpp>
#include <Time.hpp>
#include <ImageLoad.h>

#include <utils/Misc.hpp>

#include <glad/glad.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <stdexcept>
#include <cstdint>
#include <utility>

namespace fs = std::filesystem;

namespace Fennton::Flood {
    // Each layer of the scene.
    class Layer {
        // CPU-side texture data.
        std::vector<std::uint8_t> data;
        // GPU-side texture data.
        Texture tex;
        // Constructs the layer with 
        Layer(Texture&& tex) : tex(std::move(tex)) {}
    };

    static Strong<Window> mainWindow = nullptr;
    static Program* screenProg = nullptr;

    // Light contribution.
    static Texture* /* light */screenTex = nullptr;
    // Obstacles and semi-transparent cells.
    static Texture* opacityTex = nullptr;
    // The texture used by the obstacles, so that they can be told apart from the light.
    static Texture* obstacleTex = nullptr;

    // static Scoped<Layer> lightLayer;
    // static Scoped<Layer> opacityLayer;

    static std::vector<std::uint8_t> /* light */texData = {};
    static std::vector<std::uint8_t> opacityData = {};

    Time::Timer lightTimer;
    Time::Timer updateTimer;

    static Settings settings = {};

    static std::string readFromPath(const fs::path& path) {
        if (!fs::is_regular_file(path)) {
            throw std::runtime_error(std::format(
                "[Filesystem] {} is not a file.", Fennton::quote(path.generic_string())
            ));
        }
        std::fstream _file;
        _file.exceptions(std::ios::failbit | std::ios::badbit);
        _file.open(path);

        std::stringstream _ss;
        _ss << _file.rdbuf();
        return _ss.str();
    }

    static void init();
    static void term();
    static void loop();
    static void tick();
    static void render();

    static void setTextureScale(glm::ivec2 const& windowSize);

    int main(int argc, char** argv) {
        try {
            init();
        } catch (...) {
            term();
            throw;
        }
        term();
        return 0;
    }

    static void init() {
        Time::init();
        Resources::init();
        // Launcher::init();
        Window::init();
        Monitor::init();

        settings = Settings::load();

        mainWindow = Window::create(
            800, 600,
            "Fennton Flood",
            nullptr, // Does not start on fullscreen mode.
            nullptr // Not sharing the context with any other window.
        );
        // mainWindow->Maximise();
        // mainWindow->SetMonitor(Monitor::getPrimary());
        mainWindow->MakeContextCurrent();
        Window::loadGraphicsFunctions();

        // Generates the quad mesh only after loading the OpenGL functions.
        Resources::initQuad();

        texData = std::vector<std::uint8_t>(settings.width * settings.height * 3);

        std::uint8_t* _ptr = texData.data();
        for (std::int32_t y = 0; y < settings.height; ++y) {
            for (std::int32_t x = 0; x < settings.width; ++x) {

                // std::int32_t _px = x + y * settings.width * 3;
                // std::uint8_t _val = (x * 255) / settings.width;

                *(_ptr++) = 0;
                *(_ptr++) = 0;
                *(_ptr++) = 0;
            }
        }

        for (Light const& li : settings.lights) {
            std::size_t _idx = (li.position.x + li.position.y * settings.width) * 3;
            texData[_idx] = static_cast<std::uint8_t>(li.colour.x * 255.0f);
            texData[_idx + 1] = static_cast<std::uint8_t>(li.colour.y * 255.0f);
            texData[_idx + 2] = static_cast<std::uint8_t>(li.colour.z * 255.0f);
        }

        Program _screenProg = Program(
            readFromPath(Resources::getPath("shaders/screen.vert")).c_str(),
            readFromPath(Resources::getPath("shaders/screen.frag")).c_str()
        );

        Texture _screenTex = Texture(
            settings.width, settings.height, texData.data(),
            Texture::Format::cRGB,
            Texture::WrappingMode::ClampToEdge,
            Texture::WrappingMode::ClampToEdge,
            Texture::Filter::Nearest,
            Texture::Filter::Linear,
            Texture::Filter::None
        );

        // It is fine to store its address, since it is only going to be used before the init 
        // function returns.
        screenProg = &_screenProg;
        screenTex = &_screenTex;

        {
            screenProg->Use();
            std::int32_t _loc = glGetUniformLocation(screenProg->GetID(), "lightTex");
            // Only sets if the location is valid.
            if (_loc >= 0) {
                // Assigns "screenTex" the 0 texture index.
                glUniform1i(_loc, 0);
            }
        }

        mainWindow->SetOnFramebufferSizeChange(setTextureScale);
        mainWindow->Recall();

        // Updates the time to ensure the first frame behaves correctly.
        Time::update();

        lightTimer.SetTime(Time::getTime());
        lightTimer.SetRate(settings.updateRate);
        lightTimer.SetMaxRate(Time::Seconds(1));

        updateTimer.SetTime(Time::getTime());
        updateTimer.SetRate(Time::Ms(200));
        updateTimer.SetMaxRate(Time::Seconds(200));

        while (!mainWindow->ShouldClose()) {
            // Must be called before Window::pollEvents, else the the pulses immediately 
            // disappear.
            Window::updateInput();
            Window::pollEvents();
            loop();
            static std::int64_t _idx = 0;
            static std::uint8_t r = 0, g = 0, b = 0;
            bool _hasChanged = false;
            lightTimer.loop(Time::getDeltaTime(), [&_hasChanged](){
                // std::size_t _idx = (li.position.x + li.position.y * settings.width) * 3;
                texData[_idx] = r;
                texData[_idx + 1] = g;
                texData[_idx + 2] = b;
                _idx += 3;
                if (_idx >= texData.size()) {
                    _idx = 0;
                    r += 48;
                    g += 24;
                    b += 12;
                }

                _hasChanged = true;
                // Fennton::printlnf("_idx = {}", _idx);
            });
            if (_hasChanged) {
                screenTex->SetData(texData.data());
            }
            // updateTimer.loop(Time::getDeltaTime(), [&_hasChanged](){
            //     screenTex->SetData(texData.data());
            // });
            render();
            mainWindow->SwapBuffers();
            // Updates the time.
            Time::update();
        }
    }
    static void term() {
        screenProg = nullptr;
        mainWindow = nullptr;
        Window::term();
        Monitor::term();
    }
    static void loop() {
        // Toggles fullscreen mode.
        if (mainWindow->GetKeyPulseIn(Window::Key::Key_F11)) {
            if (mainWindow->HasMonitor()) {
                mainWindow->UnsetMonitor();
            } else {
                mainWindow->SetMonitor(Monitor::getPrimary());
            }
        }
    }
    static void tick() {
        
    }
    static void render() {
        glm::ivec2 _framebufferSize = mainWindow->GetFramebufferSize();
        glViewport(
            0, 0,
            _framebufferSize.x, _framebufferSize.y
        );
        glClearColor(0.2f, 0.05f, 0.025f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        screenProg->Use();
        screenTex->BindTo(0);

        // Sets the polygon drawing mode.
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // Bind the fullscreen quad's VAO.
        glBindVertexArray(Resources::getQuadVAO());
        // Draws the framebuffer's fullscreen quad.
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Unbinds the vertex array.
        glBindVertexArray(0);
    }
    static void setTextureScale(glm::ivec2 const& windowSize) {
        {
            screenProg->Use();
            std::int32_t _loc = glGetUniformLocation(screenProg->GetID(), "scale");
            // Only sets if the location is valid.
            if (_loc >= 0) {
                float
                    _texRatio =
                        static_cast<float>(screenTex->GetWidth())
                        /  static_cast<float>(screenTex->GetHeight())
                    , _windowRatio =
                        static_cast<float>(windowSize.x)
                        / static_cast<float>(windowSize.y)
                ;

                glm::vec2 _scale = glm::vec2(
                    _windowRatio > _texRatio? _texRatio / _windowRatio : 1.0f,
                    _windowRatio < _texRatio? _windowRatio / _texRatio : 1.0f
                );
                // Updates the scale for the fullscreen quad, to make sure the aspect ration 
                // stays constant even if the window's changes.
                glUniform2f(_loc, _scale.x, _scale.y);
            }
        }
    }
}