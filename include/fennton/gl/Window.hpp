#ifndef FENNTON_GL_WINDOW_HPP
#define FENNTON_GL_WINDOW_HPP

#include <fennton/utils/Memory.hpp>

struct GLFWwindow;

namespace Fennton::Gl {
    class Monitor;

    class Window {
    private:
        GLFWwindow* handle;
        // Returns the GLFW handle. Private, because support for other windowing frameworks 
        // is planned.
        GLFWwindow* GetHandle();
    public:
        // Constructs a Window object from a GLFW handle. It is recommended to call 
        // Window::create instead of constructing windows manually, specially since the 
        // backend is going to be different for platforms not supported by GLFW.
        Window(GLFWwindow* handle);
        // Initialises the window-management library and environment.
        static void init();
        // Terminates the window-management library and environment.
        static void term();
        // Creates a window with the specified width, height and name. If monitor is not null, 
        // starts the window in fullscreen mode. If share is not null, the new window will 
        // share the objects in its graphics context with share's context.
        static Memory::Strong<Window> create(
            std::int32_t width, std::int32_t height,
            char const* name,
            Memory::Strong<Monitor> const& monitor,
            Memory::Strong<Window> const& share
        );
        // Processes the window events for all windows, including input and resizing.
        static void pollEvents();
        // Makes the window's graphics context the current one.
        void MakeContextCurrent();
        // Returns true if the window should close (if the user closed it manually, for 
        // instance).
        bool ShouldClose();
        // Swaps the window's buffers for drawing.
        void SwapBuffers();
        // Destroys the window, invalidating its handle.
        void Destroy();
    };
    class Monitor {
    };

    // Initialises the OpenGL function pointers. Calling MakeContextCurrent on a window before
    // calling this function is necessary, as the function pointers might change between contexts.
    void init();
}
#endif