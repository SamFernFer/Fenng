#include <Settings.hpp>
#include <Resources.hpp>
#include <utils/JSON.hpp>

namespace json = boost::json;
namespace Fennton {
    static std::vector<Light> loadLights(json::array const& arr) {
        std::vector<Light> _lights;
        for (json::value const& v : arr) {
            json::array const& _posVal = v.at("position").as_array();
            json::array const& _colourVal = v.at("colour").as_array();

            _lights.emplace_back(Light{
                .position = glm::ivec2(
                    _posVal.at(0).to_number<std::int64_t>(), 
                    _posVal.at(1).to_number<std::int64_t>()
                ),
                .colour = glm::vec3(
                    _colourVal.at(0).to_number<float>(),
                    _colourVal.at(1).to_number<float>(),
                    _colourVal.at(2).to_number<float>()
                )
            });
        }
        return std::move(_lights);
    }
    Settings Settings::load() {
        const json::object _obj = parseFromPath(Resources::getPath("settings.json")).as_object();
        return {
            .width = _obj.at("width").to_number<std::int32_t>(),
            .height = _obj.at("height").to_number<std::int32_t>(),
            .updateRate = Time::TimeDuration(Time::Us(_obj.at("updateRate").to_number<std::int64_t>())),
            .lights = loadLights(_obj.at("lights").as_array())
        };
    }
}