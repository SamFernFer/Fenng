#ifndef FENNTON_GL_WINDOW_HPP
#define FENNTON_GL_WINDOW_HPP

#include <fennton/utils/Memory.hpp>

struct GLFWwindow;
struct GLFWmonitor;

namespace Fennton::Gl {
    class Monitor;

    class Window {
    private:
        GLFWwindow* handle;
        std::int32_t width, height;
        std::int32_t xPos, yPos;
        bool isFullscreen;
        // Returns the GLFW handle for the window. Private, because support for other windowing 
        // frameworks is planned.
        GLFWwindow* GetHandle();
    public:
        // Constructs a Window object from a GLFW window handle. It is recommended to use 
        // Window::create to get a Strong<Window> instead, as the constructor is prone to 
        // change based on the backend.
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
        // Returns true if any window has a current context. A current context is required to 
        // be able to initialise the graphics library.
        static bool hasCurrentContext();
        // Makes the window's graphics context the current one.
        void MakeContextCurrent();
        // Returns true if the window should close (if the user closed it manually, for 
        // instance).
        bool ShouldClose();
        // Marks the window for closing.
        void SetShouldClose(bool shouldClose);
        // Returns true if the window is iconified.
        bool IsIconified();
        // Swaps the window's buffers for drawing.
        void SwapBuffers();
        // Hides the window, making it invisible and removing it from the taskbar.
        void Hide();
        // Shows the window, making it visible and adding it to the taskbar.
        void Show();
        // Maximises the window.
        void Maximise();
        // Restores the window to the previous size before maximisation, iconification or going 
        // fullscreen.
        void Restore();
        // Iconifies/minimises the window.
        void Iconify();
        // Destroys the window, invalidating its handle.
        void Destroy();
        // Sets the window's monitor, making the window windowed fullscreen on it and keeping 
        // its video mode. If the monitor is null the window becomes windowed and its previous 
        // monitor gets its previous video mode restored.
        void SetMonitor(Memory::Strong<Monitor> const& monitor);
        // Returns true if the window is fullscreen on any monitor and false otherwise.
        bool IsFullscreen();
    };
    class Monitor {
        friend class Window;
    private:
        inline static Memory::Strong<Monitor> primary = nullptr;
        // The monitor's handle.
        GLFWmonitor* handle;
        // The monitor's width and height.
        std::int32_t width, height;
        // The bit depth for each colour channel.
        std::int32_t redBits, greenBits, blueBits;
        // The monitor's refresh rate.
        std::int32_t refreshRate;
        // The callback when a monitor is connected or disconnected.
        static void monitorCallback(GLFWmonitor* monitor, std::int32_t event);
        // Creates a Monitor object wrapping a GLFW monitor pointer.
        static Memory::Strong<Monitor> create(GLFWmonitor* handle);
        // Returns the GLFW handle for the monitor. Private, because support for other windowing 
        // frameworks is planned.
        GLFWmonitor* GetHandle();
    public:
        // Constructs a Monitor object from a GLFW monitor handle. It is recommended to use 
        // Monitor::GetPrimary, Monitor::GetMonitors, Monitor::GetMonitor and similar functions 
        // to get Strong<Monitor> objects instead, as the constructor is prone to change 
        // based on the backend.
        Monitor(GLFWmonitor* handle);
        static void init();
        static void term();
        // Returns the primary monitor, or null if none is found or an error happens.
        static Memory::Strong<Monitor> GetPrimary();
        // Returns the monitor's width.
        std::int32_t GetWidth() const;
        // Returns the monitor's height.
        std::int32_t GetHeight() const;
        // Returns the monitor's refresh rate.
        std::int32_t GetRefreshRate() const;
    };

    // Initialises the OpenGL function pointers. Calling MakeContextCurrent on a window before
    // calling this function is necessary, as the function pointers might change between contexts.
    void init();
}
#endif