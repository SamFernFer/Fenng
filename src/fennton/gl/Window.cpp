#include <fennton/gl/Window.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace Fennton::Memory;

namespace Fennton::Gl {
    Window::Window(GLFWwindow* handle) {
        this->handle = handle;
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
    void Monitor::monitorCallback(GLFWmonitor* monitor, std::int32_t event) {
        switch (event) {
            case GLFW_CONNECTED:
                break;
            case GLFW_DISCONNECTED:
                break;
            default:
                break;
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
        GLFWmonitor* _handle = glfwGetPrimaryMonitor();
        if (_handle) {
            primary = create(_handle);
        }
    }
    void Monitor::term() {
        glfwSetMonitorCallback(NULL);
        primary = nullptr;
    }
    Strong<Monitor> Monitor::GetPrimary() {
        return primary;
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