#include <fennton/grafik/Shader.hpp>

namespace Fennton::Grafik {
    Stage Stage::create(Stage::Type type, std::string_view src) {
        Stage _stage;
        _stage.SetSource(src);
        return _stage;
    }
}