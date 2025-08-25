#include <fennton/skript/Parser.hpp>
#include <utility>
#include <type_traits>
#include <stdexcept>

namespace Fennton::Skript {
    namespace Tokeniser {
        bool Name::operator==(Name const& other) const {
            return spelling == other.spelling;
        }
        bool Name::operator!=(Name const& other) const {
            return !(*this == other);
        }
        std::string Name::GetSpelling() const {
            throw std::runtime_error("Not implemented yet.");
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
        std::string Number::GetSpelling() const {
            throw std::runtime_error("Not implemented yet.");
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
        std::string String::GetSpelling() const {
            throw std::runtime_error("Not implemented yet.");
        }

        bool Punct::operator==(Punct const& other) const {
            return false;
        }
        bool Punct::operator!=(Punct const& other) const {
            return !(*this == other);
        }
        std::string Punct::GetSpelling() const {
            throw std::runtime_error("Not implemented yet.");
        }

        std::string Token::GetSpelling() const {
            return std::visit([](auto&& arg){
                return arg.GetSpelling();
            }, var);
        }
        constexpr bool Token::HasSpaceAfter() {
            return flags & (1<<spaceAfterBit);
        }
    }
    std::deque<Tokeniser::Token> tokenise(std::string_view str) {
        return {};
    }
}