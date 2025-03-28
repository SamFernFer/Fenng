#ifndef FENNTON_SETTINGS_HPP
#define FENNTON_SETTINGS_HPP

#include <Time.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace Fennton {
    struct Light {
        glm::ivec2 position;
        glm::vec3 colour;
    };
    struct Settings {
    public:
        std::int32_t width, height;
        // Update rate, specified in milliseconds in the JSON file.
        Time::TimeDuration updateRate;
        std::vector<Light> lights;

        static Settings load();
    };
}

#endif