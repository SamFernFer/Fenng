#include <fennton/skript/Parser.hpp>

namespace Fennton::Skript {
    namespace Tokeniser {
        std::int32_t Number::GetBase() const {
            return base;
        }
        std::vector<std::string_view> const& Number::GetParts() const {
        }
        std::vector<std::string_view> const& Number::GetSuffixes() const {
        }
    }
    std::deque<Tokeniser::Token> tokenise(std::string_view str) {
        return {};
    }
}