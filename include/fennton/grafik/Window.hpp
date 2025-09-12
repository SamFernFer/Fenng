#ifndef FENNTON_GRAFIK_WINDOW_HPP
#define FENNTON_GRAFIK_WINDOW_HPP

#include <fennton/utils/Memory.hpp>
#include <list>

struct GLFWwindow;
struct GLFWmonitor;

namespace Fennton::Grafik {
    class Monitor;

    class Window : public Memory::EnableStrongFromThis<Window> {
    private:
        GLFWwindow* handle;
        // Current dimensions of the window.
        std::int32_t width, height;
        // Dimensions of the window when windowed. Used to restore the window to its 
        // previous dimensions when getting out of fullscreen mode.
        std::int32_t wWidth, wHeight;
        // Current dimensions of the window's framebuffer.
        std::int32_t fbWidth, fbHeight;
        // Current position of the window.
        std::int32_t xPos, yPos;
        // Position of the window when windowed. Used to restore the window to its previous 
        // position when getting out of fullscreen mode.
        std::int32_t wXPos, wYPos;

        // Callback when a window's size changes.
        static void sizeCallback(GLFWwindow* handle, std::int32_t width, std::int32_t height);
        // Callback when a window's framebuffer size changes.
        static void framebufferSizeCallback(
            GLFWwindow* handle, std::int32_t width, std::int32_t height
        );
        // Callback when a window's position changes.
        static void positionCallback(GLFWwindow* handle, std::int32_t xPos, std::int32_t yPos);
        // Returns a strong pointer to a Window object from its GLFW handle.
        static Memory::Strong<Window> fromHandle(GLFWwindow* handle);
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
        // If the window is fullscreen, returns the monitor it is fullscreen on, else returns 
        // null.
        Memory::Strong<Monitor> GetMonitor() const;
        // Returns the width of the window.
        std::int32_t GetWidth() const;
        // Returns the height of the window.
        std::int32_t GetHeight() const;
        // Returns the width of the window when windowed.
        std::int32_t GetWindowedWidth() const;
        // Returns the height of the window when windowed.
        std::int32_t GetWindowedHeight() const;
        // Returns the width of the window's framebuffer.
        std::int32_t GetFramebufferWidth() const;
        // Returns the height of the window's framebuffer.
        std::int32_t GetFramebufferHeight() const;
        // Returns the position of the window in the X axis.
        std::int32_t GetXPosition() const;
        // Returns the position of the window in the Y axis.
        std::int32_t GetYPosition() const;
        // Returns the windowed position of the window in the X axis.
        std::int32_t GetWindowedXPosition() const;
        // Returns the windowed position of the window in the Y axis.
        std::int32_t GetWindowedYPosition() const;
    };
    class Monitor : public Memory::EnableStrongFromThis<Monitor> {
        friend class Window;
    private:
        inline static std::list<Memory::Strong<Monitor>> monitors;
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
        // Returns a strong pointer to a Monitor object from its GLFW handle.
        static Memory::Strong<Monitor> fromHandle(GLFWmonitor* handle);
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
        // Returns the monitor at specified index. Throws if the index is out of bounds.
        static Memory::Strong<Monitor> GetMonitor(std::int32_t index);
        // Returns the number of connected monitors.
        static std::int32_t GetMonitorCount();
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