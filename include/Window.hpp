#ifndef FENNTON_WINDOW_HPP
#define FENNTON_WINDOW_HPP

#include <Memory.hpp>
#include <glm/glm.hpp>

#include <unordered_map>
#include <functional>
#include <cstdint>

struct GLFWwindow;
struct GLFWmonitor;
struct GLFWvidmode;

namespace Fennton {
    class Monitor;
    class Window;

    class Monitor {
        friend Window;
        inline static Strong<Monitor> primary = nullptr;

        GLFWmonitor* handle = NULL;
        const GLFWvidmode* videoMode = NULL;
    public:
        Monitor(GLFWmonitor* handle);
        Monitor() = delete;
        Monitor(Monitor const&) = delete;
        Monitor(Monitor&&) = delete;
        Monitor& operator=(Monitor const&) = delete;
        Monitor& operator=(Monitor&&) = delete;
        ~Monitor();

        // Initialises the primary monitor.
        static void init();
        // Destroys the primary monitor object.
        static void term();
        // Returns the primary monitor. Useful for making a window fullscreen.
        static Strong<Monitor> getPrimary();
        // Destroys the monitor.
        void Destroy();
        // Returns the handle used by the application's window manager for the monitor.
        void* GetHandle();
        std::int32_t GetWidth() const;
        std::int32_t GetHeight() const;
        std::int32_t GetRefreshRate() const;
    };
    class Window {
    public:
        enum class Key {
            Key_UNKNOWN = -1,
            Key_SPACE = 32,
            Key_APOSTROPHE = 39,  /* ' */
            Key_COMMA = 44,  /* , */
            Key_MINUS = 45,  /* - */
            Key_PERIOD = 46,  /* . */
            Key_SLASH = 47,  /* / */
            Key_0 = 48,
            Key_1 = 49,
            Key_2 = 50,
            Key_3 = 51,
            Key_4 = 52,
            Key_5 = 53,
            Key_6 = 54,
            Key_7 = 55,
            Key_8 = 56,
            Key_9 = 57,
            Key_SEMICOLON = 59,  /* ; */
            Key_EQUAL = 61,  /* = */
            Key_A = 65,
            Key_B = 66,
            Key_C = 67,
            Key_D = 68,
            Key_E = 69,
            Key_F = 70,
            Key_G = 71,
            Key_H = 72,
            Key_I = 73,
            Key_J = 74,
            Key_K = 75,
            Key_L = 76,
            Key_M = 77,
            Key_N = 78,
            Key_O = 79,
            Key_P = 80,
            Key_Q = 81,
            Key_R = 82,
            Key_S = 83,
            Key_T = 84,
            Key_U = 85,
            Key_V = 86,
            Key_W = 87,
            Key_X = 88,
            Key_Y = 89,
            Key_Z = 90,
            Key_LEFT_BRACKET = 91,  /* [ */
            Key_BACKSLASH = 92,  /* \ */
            Key_RIGHT_BRACKET = 93,  /* ] */
            Key_GRAVE_ACCENT = 96,  /* ` */
            Key_WORLD_1 = 161, /* non-US #1 */
            Key_WORLD_2 = 162, /* non-US #2 */

            /* Function keys */
            Key_ESCAPE = 256,
            Key_ENTER = 257,
            Key_TAB = 258,
            Key_BACKSPACE = 259,
            Key_INSERT = 260,
            Key_DELETE = 261,
            Key_RIGHT = 262,
            Key_LEFT = 263,
            Key_DOWN = 264,
            Key_UP = 265,
            Key_PAGE_UP = 266,
            Key_PAGE_DOWN = 267,
            Key_HOME = 268,
            Key_END = 269,
            Key_CAPS_LOCK = 280,
            Key_SCROLL_LOCK = 281,
            Key_NUM_LOCK = 282,
            Key_PRINT_SCREEN = 283,
            Key_PAUSE = 284,
            Key_F1 = 290,
            Key_F2 = 291,
            Key_F3 = 292,
            Key_F4 = 293,
            Key_F5 = 294,
            Key_F6 = 295,
            Key_F7 = 296,
            Key_F8 = 297,
            Key_F9 = 298,
            Key_F10 = 299,
            Key_F11 = 300,
            Key_F12 = 301,
            Key_F13 = 302,
            Key_F14 = 303,
            Key_F15 = 304,
            Key_F16 = 305,
            Key_F17 = 306,
            Key_F18 = 307,
            Key_F19 = 308,
            Key_F20 = 309,
            Key_F21 = 310,
            Key_F22 = 311,
            Key_F23 = 312,
            Key_F24 = 313,
            Key_F25 = 314,
            Key_KP_0 = 320,
            Key_KP_1 = 321,
            Key_KP_2 = 322,
            Key_KP_3 = 323,
            Key_KP_4 = 324,
            Key_KP_5 = 325,
            Key_KP_6 = 326,
            Key_KP_7 = 327,
            Key_KP_8 = 328,
            Key_KP_9 = 329,
            Key_KP_DECIMAL = 330,
            Key_KP_DIVIDE = 331,
            Key_KP_MULTIPLY = 332,
            Key_KP_SUBTRACT = 333,
            Key_KP_ADD = 334,
            Key_KP_ENTER = 335,
            Key_KP_EQUAL = 336,
            Key_LEFT_SHIFT = 340,
            Key_LEFT_CONTROL = 341,
            Key_LEFT_ALT = 342,
            Key_LEFT_SUPER = 343,
            Key_RIGHT_SHIFT = 344,
            Key_RIGHT_CONTROL = 345,
            Key_RIGHT_ALT = 346,
            Key_RIGHT_SUPER = 347,
            Key_MENU = 348,

            Key_LAST = Key_MENU
        };
        enum class KeyState {
            Out = 0, // Released for more than one frame.
            PulseOut = 1, // Just released.
            PulseIn = 2, // Just pressed.
            In = 3 // Held down for more than one frame.
        };
        using KeyMap = std::unordered_map<Key, KeyState>;
        using WindowMap = std::unordered_map<GLFWwindow*, Weak<Window>>;

        using SizeCallback = std::function<void(glm::ivec2 const& size)>;
        using PosCallback = std::function<void(glm::ivec2 const& position)>;
    private:
        inline static WindowMap windowMap = {};
        
        GLFWwindow* handle = NULL;
        glm::ivec2
            position = glm::ivec2(0),
            size = glm::ivec2(0),
            framebufferSize = glm::ivec2(0)
        ;
        // Saves position and size for when the window becomes fullscreen so that it can be 
        // brought back to how it was before.
        glm::ivec2
            lastPosition = glm::ivec2(0),
            lastSize = glm::ivec2(0)
        ;

        SizeCallback onSizeChange = nullptr, onFramebufferSizeChange = nullptr;
        PosCallback onPositionChange = nullptr;
        // Map of key to current key state.
        // For the value: 0 is release, 1 is pressed once and 2 is held down.
        KeyMap keyStates = {};
        // The callback each created window registers to.
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void sizeCallback(GLFWwindow* window, int width, int height);
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        static void posCallback(GLFWwindow* window, int xpos, int ypos);
        // Returns a Window object from its handle.
        static Strong<Window> getWindowByHandle(GLFWwindow* handle);
        // Updates the window's size and executes the registered callback, if any.
        void UpdateSize(int width, int height);
        // Updates the window's framebuffer size and executes the registered callback, if any.
        void UpdateFramebufferSize(int width, int height);
        // Updates the window's position and executes the registered callback, if any.
        void UpdatePosition(int xpos, int ypos);
        // Updates the state of the specified key.
        void UpdateKeyState(Key key, std::int32_t action);
    public:
        // Constructs a Window object with a GLFW window handle.
        Window(GLFWwindow* handle);
        Window() = delete;
        Window(Window const&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window const&) = delete;
        Window& operator=(Window&&) = delete;
        ~Window();

        // Initialises the window-manager, which interfaces with the OS's.
        static void init();
        // Terminates the window-manager, which interfaces with the OS's.
        static void term();
        // Updates the input state of every window. The order is not necessarily and order in 
        // which they were created.
        static void updateInput();
        // Creates a window with the specified size, title, monitor (for full-screen mode) and 
        // window to share the graphics context with.
        static Strong<Window> create(
            std::int32_t width, std::int32_t height, // Size.
            const char* title, // The window's title.
            Strong<Monitor> const& monitor = nullptr, // Does not start on fullscreen mode.
            Strong<Window> const& window = nullptr // Not sharing the context with any other window.
        );
        // Polls the window events.
        static void pollEvents();
        // Loads the OpenGL functions. Must be called after calling MakeContextCurrent on the 
        // window you wish to use OpenGL on.
        static void loadGraphicsFunctions();
        // Calls all of the window's registered callbacks, except the input-related ones and 
        // updates the window's related cached state.
        void Touch();
        // Calls all of the window's registered callbacks, except the input-related ones.
        void Recall();
        // Sets the callback for when the window's size changes.
        void SetOnSizeChange(SizeCallback const& callback);
        // Sets the callback for when the window's framebuffer size changes.
        void SetOnFramebufferSizeChange(SizeCallback const& callback);
        // Sets the callback for when the window's position changes.
        void SetOnPositionChange(PosCallback const& callback);
        // Returns the callback for when the window's size changes.
        SizeCallback GetOnSizeChange() const;
        // Returns the callback for when the window's framebuffer size changes.
        SizeCallback GetOnFramebufferSizeChange() const;
        // Returns the callback for when the window's position changes.
        PosCallback GetOnPositionChange() const;
        // Updates the window's input state.
        void UpdateInput();
        // Returns the window's position. Throws if the window has been destroyed or its 
        // handle is null.
        glm::ivec2 GetPosition() const;
        // Returns the window's size. Throws if the window has been destroyed or its 
        // handle is null.
        glm::ivec2 GetSize() const;

        // Returns the window's width. Throws if the window has been destroyed or its 
        // handle is null.
        // std::int32_t GetWidth() const;
        // Returns the window's height. Throws if the window has been destroyed or its 
        // handle is null.
        // std::int32_t GetHeight() const;

        // Returns the window's framebuffer's size. Throws if the window has been destroyed or its 
        // handle is null.
        glm::ivec2 GetFramebufferSize() const;

        // Returns the window's framebuffer's width. Throws if the window has been destroyed or its 
        // handle is null.
        // std::int32_t GetFramebufferWidth() const;
        // Returns the window's framebuffer's height. Throws if the window has been destroyed or its 
        // handle is null.
        // std::int32_t GetFramebufferHeight() const;

        // Returns the handle used by the application's window manager for the window.
        void* GetHandle() const;
        // Destroys the window. Calling on an already-destroyed window does nothing. Automatically 
        // called inside the destructor.
        void Destroy();
        // Checks if the window was told to close. Throws if the window has been destroyed or its 
        // handle is null.
        bool ShouldClose() const;
        // Maximises the window. Throws if the window has been destroyed or its 
        // handle is null.
        void Maximise();
        // Sets the window's monitor, making it fullscreen on that monitor. Use UnsetMonitor 
        // to make a fullscreen window windowed. Throws if the monitor is null, if the window 
        // has been destroyed or if the window's handle is null.
        void SetMonitor(Strong<Monitor> const& monitor);
        // Unsets the window's monitor. Does nothing if the window does not have a monitor. 
        // Throws if the window has been destroyed or its handle is null.
        void UnsetMonitor();
        // Returns true if the window is fullscreen on any monitor, else returns false. 
        // Throws if the window has been destroyed or its handle is null.
        bool HasMonitor();
        // Swaps the window's buffers. Should be called to prevent the window from freezing, even 
        // if not using any event directly. Throws if the window has been destroyed or its 
        // handle is null.
        void SwapBuffers();
        // Makes the window's graphics context current. Must be called once before using OpenGL 
        // functions and before loading the OpenGL functions with Glad.
        void MakeContextCurrent();
        // Returns the key's current state. Returns KeyState::Out if the key is .
        KeyState GetKeyState(Key key);
        // Returns true if the key has JUST been pressed.
        bool GetKeyPulseIn(Key key);
        // Returns true if the key has JUST been pressed.
        bool GetKeyPulseOut(Key key);
        // Returns true if the key is being pressed. Overlaps GetKeyPulseIn too.
        bool GetKeyIn(Key key);
        // Returns true if the key has been released. Overlaps GetKeyPulseOut too.
        bool GetKeyOut(Key key);
    };
}
#endif