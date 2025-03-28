#include <Window.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

namespace Fennton {
    void Monitor::init() {
        primary = makeStrong<Monitor>(glfwGetPrimaryMonitor());
    }
    void Monitor::term() {
        primary = nullptr;
    }
    Strong<Monitor> Monitor::getPrimary() {
        return primary;
    }
    Monitor::Monitor(GLFWmonitor* handle) {
        if (handle == NULL) {
            throw std::runtime_error(
                "Monitor: Cannot construct monitor with null handle."
            );
        }
        this->handle = handle;
        this->videoMode = glfwGetVideoMode(handle);
    }
    Monitor::~Monitor() {
        Destroy();
    }
    void Monitor::Destroy() {
        handle = NULL;
        videoMode = NULL;
    }
    void* Monitor::GetHandle() {
        return handle;
    }
    std::int32_t Monitor::GetWidth() const {
        if (handle == NULL) {
            throw std::runtime_error(
                "Monitor: Monitor::GetWidth cannot be called on a null monitor."
            );
        }
        return videoMode->width;
    }
    std::int32_t Monitor::GetHeight() const {
        if (handle == NULL) {
            throw std::runtime_error(
                "Monitor: Monitor::GetWidth cannot be called on a null monitor."
            );
        }
        return videoMode->height;
    }
    std::int32_t Monitor::GetRefreshRate() const {
        if (handle == NULL) {
            throw std::runtime_error(
                "Monitor: Monitor::GetWidth cannot be called on a null monitor."
            );
        }
        return videoMode->refreshRate;
    }
    void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        Strong<Window> _win = getWindowByHandle(window);
        if (_win) {
            _win->UpdateKeyState(static_cast<Window::Key>(key), action);
        }
    }
    void Window::sizeCallback(GLFWwindow* window, int width, int height) {
        Strong<Window> _win = getWindowByHandle(window);
        if (_win) {
            _win->UpdateSize(width, height);
        }
    }
    void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        Strong<Window> _win = getWindowByHandle(window);
        if (_win) {
            _win->UpdateFramebufferSize(width, height);
        }
    }
    void Window::posCallback(GLFWwindow* window, int xpos, int ypos) {
        Strong<Window> _win = getWindowByHandle(window);
        if (_win) {
            _win->UpdatePosition(xpos, ypos);
        }
    }
    Strong<Window> Window::getWindowByHandle(GLFWwindow* handle) {
        auto _find = windowMap.find(handle);
        if (_find == windowMap.end()) {
            return nullptr;
        } else {
            return _find->second.lock();
        }
    }
    void Window::UpdateSize(int width, int height) {
        size = glm::ivec2(width, height);
        if (onSizeChange) onSizeChange(size);
    }
    void Window::UpdateFramebufferSize(int width, int height) {
        framebufferSize = glm::ivec2(width, height);
        if (onFramebufferSizeChange) onFramebufferSizeChange(framebufferSize);
    }
    void Window::UpdatePosition(int xpos, int ypos) {
        position = glm::ivec2(xpos, ypos);
        if (onPositionChange) onPositionChange(position);
    }
    void Window::UpdateKeyState(Key key, std::int32_t action) {
        // Registers the press and release pulses.
        switch (action) {
            case GLFW_PRESS:
                keyStates[key] = KeyState::PulseIn;
                break;
            case GLFW_RELEASE:
                keyStates[key] = KeyState::PulseOut;
                break;
        }
    }
    Window::Window(GLFWwindow* handle) {
        this->handle = handle;

        glfwSetKeyCallback(handle, Window::keyCallback);
        glfwSetWindowSizeCallback(handle, Window::sizeCallback);
        glfwSetFramebufferSizeCallback(handle, Window::framebufferSizeCallback);
        glfwSetWindowPosCallback(handle, Window::posCallback);

        Touch();
    }
    Window::~Window() {
        Destroy();
    }
    void Window::init() {
        // Initialises GLFW.
        if (!glfwInit()) {
            throw std::runtime_error("Window: Failed to initialise GLFW.");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif
    }
    void Window::term() {
        // Tries to free the memory blocks of the weak pointers.
        windowMap = {};
        // Terminates GLFW.
        glfwTerminate();
    }
    void Window::updateInput() {
        for (WindowMap::value_type const& p : windowMap) {
            if (Strong<Window> _win = p.second.lock()) {
                _win->UpdateInput();
            }
        }
    }
    Strong<Window> Window::create(
        std::int32_t width, std::int32_t height,
        const char* title,
        Strong<Monitor> const& monitor,
        Strong<Window> const& window
    ) {
        // Creates the Window object with a GLFW handle.
        Strong<Window> _newWindow = makeStrong<Window>(glfwCreateWindow(
            width, height, title,
            monitor? monitor->handle : NULL,
            window? window->handle : NULL
        ));
        windowMap.insert(std::make_pair(_newWindow->handle, _newWindow));
        // Returns the window.
        return _newWindow;
    }
    void Window::pollEvents() {
        glfwPollEvents();
    }
    void Window::loadGraphicsFunctions() {
        // Loads the OpenGL functions.
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            throw std::runtime_error("Window: Failed to load the OpenGL functions with GLAD.");
        }
    }
    void Window::Touch() {
        if (handle == NULL) {
            throw std::runtime_error(
                "Window: Window::Touch cannot be called on a null window."
            );
        }

        std::int32_t x, y;
        glfwGetWindowSize(handle, &x, &y);
        UpdateSize(x, y);

        glfwGetFramebufferSize(handle, &x, &y);
        UpdateFramebufferSize(x, y);

        glfwGetWindowPos(handle, &x, &y);
        UpdatePosition(x, y);
    }
    void Window::Recall() {
        if (handle == NULL) {
            throw std::runtime_error(
                "Window: Window::Touch cannot be called on a null window."
            );
        }

        if (onSizeChange) onSizeChange(size);
        if (onFramebufferSizeChange) onFramebufferSizeChange(framebufferSize);
        if (onPositionChange) onPositionChange(position);
    }
    void Window::SetOnSizeChange(SizeCallback const& callback) {
        onSizeChange = callback;
    }
    void Window::SetOnFramebufferSizeChange(SizeCallback const& callback) {
        onFramebufferSizeChange = callback;
    }
    void Window::SetOnPositionChange(PosCallback const& callback) {
        onPositionChange = callback;
    }
    Window::SizeCallback Window::GetOnSizeChange() const {
        return onSizeChange;
    }
    Window::SizeCallback Window::GetOnFramebufferSizeChange() const {
        return onFramebufferSizeChange;
    }
    Window::PosCallback Window::GetOnPositionChange() const {
        return onPositionChange;
    }
    void Window::UpdateInput() {
        for (KeyMap::value_type& p : keyStates) {
            switch (p.second) {
                case KeyState::PulseIn:
                    // The pulse already happended, so the key is now being held down.
                    p.second = KeyState::In;
                    break;
                case KeyState::PulseOut:
                    // The pulse already happened, so the key is now being released for 
                    // more than one frame.
                    p.second = KeyState::Out;
                    break;
                // Just so that the compiler knows that the others case were not forgotten.
                case KeyState::In:
                case KeyState::Out:
                default:
                    break;
            }
        }
    }
    glm::ivec2 Window::GetPosition() const {
        if (handle == NULL) {
            throw std::runtime_error(
                "Window: Window::GetWidth cannot be called on a null window."
            );
        }
        return position;
    }
    glm::ivec2 Window::GetSize() const {
        if (handle == NULL) {
            throw std::runtime_error(
                "Window: Window::GetWidth cannot be called on a null window."
            );
        }
        return size;
    }

    // std::int32_t Window::GetWidth() const {
    //     if (handle == NULL) {
    //         throw std::runtime_error(
    //             "Window: Window::GetWidth cannot be called on a null window."
    //         );
    //     }
    //     std::int32_t _width, _height;
    //     glfwGetWindowSize(handle, &_width, &_height);
    //     return _width;
    // }
    // std::int32_t Window::GetHeight() const {
    //     if (handle == NULL) {
    //         throw std::runtime_error(
    //             "Window: Window::GetHeight cannot be called on a null window."
    //         );
    //     }
    //     std::int32_t _width, _height;
    //     glfwGetWindowSize(handle, &_width, &_height);
    //     return _height;
    // }

    glm::ivec2 Window::GetFramebufferSize() const {
        if (handle == NULL) {
            throw std::runtime_error(
                "Window: Window::GetWidth cannot be called on a null window."
            );
        }
        return framebufferSize;
    }

    // std::int32_t Window::GetFramebufferWidth() const {
    //     if (handle == NULL) {
    //         throw std::runtime_error(
    //             "Window: Window::GetWidth cannot be called on a null window."
    //         );
    //     }
    //     std::int32_t _width, _height;
    //     glfwGetFramebufferSize(handle, &_width, &_height);
    //     return _width;
    // }
    // std::int32_t Window::GetFramebufferHeight() const {
    //     if (handle == NULL) {
    //         throw std::runtime_error(
    //             "Window: Window::GetHeight cannot be called on a null window."
    //         );
    //     }
    //     std::int32_t _width, _height;
    //     glfwGetFramebufferSize(handle, &_width, &_height);
    //     return _height;
    // }

    void* Window::GetHandle() const {
        return handle;
    }
    void Window::Destroy() {
        if (handle) {
            // Removes the window from the window map.
            windowMap.erase(handle);

            glfwDestroyWindow(handle);
            handle = NULL;
        }
    }
    bool Window::ShouldClose() const {
        if (handle == NULL) {
            throw std::runtime_error(
                "Window: Window::ShouldClose cannot be called on a null window."
            );
        }
        return glfwWindowShouldClose(handle);
    }
    void Window::Maximise() {
        if (handle == NULL) {
            throw std::runtime_error(
                "Window: Window::Maximise cannot be called on a null window."
            );
        }
        glfwMaximizeWindow(handle);
    }
    void Window::SetMonitor(Strong<Monitor> const& monitor) {
        if (handle == NULL) {
            throw std::runtime_error(
                "Window: Window::SetMonitor cannot be called on a null window."
            );
        }
        if (!monitor) {
            throw std::runtime_error(
                "Window: monitor cannot be null."
            );
        }

        // Saves the last position and size.
        lastPosition = GetPosition();
        lastSize = GetSize();

        glfwSetWindowMonitor(
            handle, monitor->handle,
            0, 0, monitor->GetWidth(), monitor->GetHeight(), monitor->GetRefreshRate()
        );
    }
    void Window::UnsetMonitor() {
        if (handle == NULL) {
            throw std::runtime_error(
                "Window: Window::SetMonitor cannot be called on a null window."
            );
        }
        // Only unsets the monitor if the window already has a monitor, to prevent setting the 
        // window's position and size to undefined values.
        if (glfwGetWindowMonitor(handle)) {
            glfwSetWindowMonitor(handle, NULL,
                lastPosition.x, lastPosition.y,
                lastSize.x, lastSize.y,
                0 // Windowed does not use the refresh rate parameter.
            );
        }
    }
    bool Window::HasMonitor() {
        if (handle == NULL) {
            throw std::runtime_error(
                "Window: Window::SetMonitor cannot be called on a null window."
            );
        }
        return glfwGetWindowMonitor(handle);
    }
    void Window::SwapBuffers() {
        if (handle == NULL) {
            throw std::runtime_error(
                "Window: Window::Maximise cannot be called on a null window."
            );
        }
        glfwSwapBuffers(handle);
    }
    void Window::MakeContextCurrent() {
        glfwMakeContextCurrent(handle);
    }
    Window::KeyState Window::GetKeyState(Key key) {
        auto _find = keyStates.find(key);
        if (_find == keyStates.end()) {
            return KeyState::Out;
        } else {
            return _find->second;
        }
    }
    bool Window::GetKeyPulseIn(Key key) {
        switch (GetKeyState(key)) {
            case KeyState::PulseIn:
                return true;
            default:
                return false;
        }
    }
    bool Window::GetKeyPulseOut(Key key) {
        switch (GetKeyState(key)) {
            case KeyState::PulseOut:
                return true;
            default:
                return false;
        }
    }
    bool Window::GetKeyIn(Key key) {
        switch (GetKeyState(key)) {
            case KeyState::In:
            case KeyState::PulseIn:
                return true;
            default:
                return false;
        }
    }
    bool Window::GetKeyOut(Key key) {
        switch (GetKeyState(key)) {
            case KeyState::Out:
            case KeyState::PulseOut:
                return true;
            default:
                return false;
        }
    }
}