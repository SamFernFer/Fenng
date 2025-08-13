#include <fennton/utils/Console.hpp>

#include <fennton/gl/Window.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <format>

using namespace Fennton::Memory;

namespace Fennton::Gl {
    Window::Window(GLFWwindow* handle) {
        this->handle = handle;
        glfwSetWindowUserPointer(handle, this);
    }
    void Window::sizeCallback(
        GLFWwindow* handle, std::int32_t width, std::int32_t height
    ) {
        Strong<Window> _win = fromHandle(handle);
        _win->width = width;
        _win->height = height;
    }
    void Window::positionCallback(
        GLFWwindow* handle, std::int32_t xPos, std::int32_t yPos
    ) {
        Strong<Window> _win = fromHandle(handle);
        _win->xPos = xPos;
        _win->yPos = yPos;
    }
    Strong<Window> Window::fromHandle(GLFWwindow* handle) {
        if (handle != NULL) {
            void* _userPtr = glfwGetWindowUserPointer(handle);
            if (_userPtr) {
                return static_cast<Window*>(_userPtr)->StrongFromThis();
            } else {
                return nullptr;
            }
        } else {
            return nullptr;
        }
    }
    GLFWwindow* Window::GetHandle() {
        return handle;
    }
    void Window::init() {
        if (!glfwInit()) {
            throw std::runtime_error("Window: Failed to initialise GLFW.");
        }
        // Sets window-creation hints.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif
        // glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    }
    void Window::term() {
        // Resets all window-creation hints.
        glfwDefaultWindowHints();
        // Terminates GLFW.
        glfwTerminate();
    }
    Strong<Window> Window::create(
        std::int32_t width, std::int32_t height,
        char const* name,
        Strong<Monitor> const& monitor,
        Strong<Window> const& share
    ) {
        GLFWwindow* _handle = glfwCreateWindow(
            width, height, name,
            NULL, // monitor? monitor->GetHandle();
            share? share->GetHandle() : NULL
        );
        if (_handle == NULL) {
            throw std::runtime_error("Window: Failed to create GLFW window.");
        }
        return makeStrong<Window>(_handle);
    }
    void Window::pollEvents() {
        glfwPollEvents();
    }
    bool Window::hasCurrentContext() {
        return glfwGetCurrentContext() != NULL;
    }
    void Window::MakeContextCurrent() {
        glfwMakeContextCurrent(handle);
    }
    bool Window::ShouldClose() {
        return glfwWindowShouldClose(handle);
    }
    void Window::SetShouldClose(bool shouldClose) {
        return glfwSetWindowShouldClose(handle, shouldClose);
    }
    bool Window::IsIconified() {
        return glfwGetWindowAttrib(handle, GLFW_ICONIFIED);
    }
    void Window::SwapBuffers() {
        glfwSwapBuffers(handle);
    }
    void Window::Hide() {
        glfwHideWindow(handle);
    }
    void Window::Show() {
        glfwShowWindow(handle);
    }
    void Window::Maximise() {
        glfwMaximizeWindow(handle);
    }
    void Window::Restore() {
        glfwRestoreWindow(handle);
    }
    void Window::Iconify() {
        glfwIconifyWindow(handle);
    }
    void Window::Destroy() {
        glfwDestroyWindow(handle);
    }
    void Window::SetMonitor(Memory::Strong<Monitor> const& monitor) {
        if (monitor) {
            glfwSetWindowMonitor(
                handle, monitor->GetHandle(),
                0, 0,
                monitor->GetWidth(), monitor->GetHeight(),
                monitor->GetRefreshRate()
            );
        } else {
            Restore();
        }
    }
    /* Strong<Monitor> Window::GetMonitor() {
        return Monitor::fromHandle(glfwGetWindowMonitor(handle));
    } */
    void Monitor::monitorCallback(GLFWmonitor* monitor, std::int32_t event) {
        switch (event) {
            case GLFW_CONNECTED:
                Console::print("Connected: ");
                break;
            case GLFW_DISCONNECTED:
                Console::print("Disconnected: ");
                break;
            default:
                break;
        }
        Console::printl(glfwGetMonitorName(monitor));
    }
    Memory::Strong<Monitor> Monitor::fromHandle(GLFWmonitor* handle) {
        if (handle != NULL) {
            void* _userPtr = glfwGetMonitorUserPointer(handle);
            if (_userPtr) {
                return static_cast<Monitor*>(_userPtr)->StrongFromThis();
            } else {
                return nullptr;
            }
        } else {
            return nullptr;
        }
    }
    Memory::Strong<Monitor> Monitor::create(GLFWmonitor* handle) {
        return makeStrong<Monitor>(handle);
    }
    GLFWmonitor* Monitor::GetHandle() {
        return handle;
    }
    Monitor::Monitor(GLFWmonitor* handle) {
        this->handle = handle;
        glfwSetMonitorUserPointer(handle, this);
        // Retrieves the monitor's video mode.
        GLFWvidmode const* _vidmode = glfwGetVideoMode(handle);
        // Stores the monitor's current video mode properties.
        this->width = _vidmode->width;
        this->height = _vidmode->height;
        this->redBits = _vidmode->redBits;
        this->greenBits = _vidmode->greenBits;
        this->blueBits = _vidmode->blueBits;
        this->refreshRate = _vidmode->refreshRate;
    }
    void Monitor::init() {
        glfwSetMonitorCallback(monitorCallback);

        std::int32_t _count;
        GLFWmonitor** _monitors = glfwGetMonitors(&_count);
        // Initialises the list of monitors with the right size.
        monitors = std::list<Strong<Monitor>>(_count);

        std::size_t i = 0;
        for (auto it = monitors.begin(); it != monitors.end(); ++it, ++i) {
            *it = create(_monitors[i]);
        }
    }
    void Monitor::term() {
        glfwSetMonitorCallback(NULL);
    }
    Strong<Monitor> Monitor::GetPrimary() {
        return fromHandle(glfwGetPrimaryMonitor());
    }
    Strong<Monitor> Monitor::GetMonitor(std::int32_t index) {
        std::int32_t _count;
        GLFWmonitor** _monitors = glfwGetMonitors(&_count);

        if (index < 0 || index >= _count) {
            throw std::runtime_error(std::format(
                "Monitor: Index {} out of bounds of the monitor list.", index
            ));
        }
        return fromHandle(_monitors[index]);
    }
    std::int32_t Monitor::GetMonitorCount() {
        std::int32_t _count;
        glfwGetMonitors(&_count);
        return _count;
    }
    std::int32_t Monitor::GetWidth() const {
        return width;
    }
    std::int32_t Monitor::GetHeight() const {
        return height;
    }
    std::int32_t Monitor::GetRefreshRate() const {
        return refreshRate;
    }
    void init() {
        if (!Window::hasCurrentContext()) {
            throw std::runtime_error("GL: Cannot initialise GLAD without a current context.");
        }
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            throw std::runtime_error("GL: Failed to initialised GLAD.");
        }
    }
}