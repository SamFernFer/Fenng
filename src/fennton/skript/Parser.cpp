#include <fennton/skript/Parser.hpp>
#include <type_traits>

namespace Fennton::Skript {
    namespace Tokeniser {
        bool Name::operator==(Name const& other) const {
            return spelling == other.spelling;
        }
        bool Name::operator!=(Name const& other) const {
            return !(*this == other);
        }

        bool Number::operator==(Number const& other) const {
            return
                (parts == other.parts)
                && (suffixes == other.suffixes)
            ;
        }
        bool Number::operator!=(Number const& other) const {
            return !(*this == other);
        }
        std::vector<std::string_view> const& Number::GetParts() const {
            return parts;
        }
        std::vector<std::string_view> const& Number::GetSuffixes() const {
            return suffixes;
        }
        std::int32_t Number::GetBase() const {
            return base;
        }

        bool String::operator==(String const& other) const {
            return false;
        }
        bool String::operator!=(String const& other) const {
            return !(*this == other);
        }

        bool Punct::operator==(Punct const& other) const {
            return false;
        }
        bool Punct::operator!=(Punct const& other) const {
            return !(*this == other);
        }

        std::string const& Token::GetSpelling() const {
            std::visit([](auto&& arg){
                using T = std::decay_t<decltype(arg)>;
                if (std::is_same<T, >) {
                } else {
                    f
                }
            }, var);
        }
        bool HasSpaceAfter() const {
            return flags & (1<<spaceAfterBit);
        }
    }
    std::deque<Tokeniser::Token> tokenise(std::string_view str) {
        return {};
    }
}