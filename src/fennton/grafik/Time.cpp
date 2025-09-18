#include <fennton/grafik/Time.hpp>
#include <GLFW/glfw3.h>

namespace Fennton::Grafik {
    double getTime() {
        return glfwGetTime();
    }
    std::uint64_t getTimerValue() {
        return glfwGetTimerValue();
    }
    std::uint64_t getTimerFrequency() {
        return glfwGetTimerFrequency();
    }
};