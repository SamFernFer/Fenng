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
    bool Window::IsIconified() {
        return glfwGetWindowAttrib(handle, GLFW_ICONIFIED);
    }
    void Window::SwapBuffers() {
        glfwSwapBuffers(handle);
    }
    void Window::Destroy() {
        glfwDestroyWindow(handle);
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