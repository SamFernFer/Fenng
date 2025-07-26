#ifndef FENNTON_GL_WINDOW_HPP
#define FENNTON_GL_WINDOW_HPP

#include <fennton/utils/Memory.hpp>

namespace Fennton::Gl {
    class Monitor;

    class Window {
    public:
        Window();
        static void init();
        static void term();
        Strong<Window> create(
            std::int32_t width, std::int32_t height,
            char const* name,
            Strong<Monitor> const& monitor,
            Strong<Window> const& window
        ) {
            ;
        }
    };
    class Monitor {
    };
}
#endif