#include <fennton/gl/Window.hpp>
#include <fennton/utils/Memory.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

namespace Fennton::Gl {
    Window::Window() {
        std::cout << "Hello, window!" << std::endl;
    }
    void Window::init() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialise GLFW.");
        }
    }
    void Window::term() {
    }
}