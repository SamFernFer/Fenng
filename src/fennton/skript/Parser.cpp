#include <fennton/skript/Parser.hpp>
#include <utility>
#include <algorithm>
#include <locale>
#include <stdexcept>
#include <cctype>

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

        Number::Number(
            std::string const& storage,
            std::vector<std::string_view> const& parts,
            std::vector<std::string_view> const& suffixes,
            std::int32_t base
        ) {
            this->storage = storage;
            this->parts = parts;
            this->suffixes = suffixes;
            this->base = base;
        }
        Number::Number(
            std::vector<std::string_view> const& parts,
            std::vector<std::string_view> const& suffixes,
            std::int32_t base
        ) {
            // The base can be set without worrying about the storage string.
            {
                std::size_t _totalSize = 0;
                for (std::string_view s : parts) { _totalSize += s.size(); }
                for (std::string_view s : suffixes) { _totalSize += s.size(); }
                // Reconstructs the storage string with the correct size, so that there is no 
                // need for multiple allocations.
                this->storage = std::string(_totalSize, '#');
            }
            this->parts = std::vector<std::string_view>(parts.size());
            this->suffixes = std::vector<std::string_view>(suffixes.size());
            this->base = base;

            std::string::iterator _it = this->storage.begin();
            for (std::size_t i = 0; i < parts.size(); ++i) {
                std::string_view _s = parts[i];

                std::string::iterator _oldIt = _it;
                _it = std::copy(_s.begin(), _s.end(), _it);

                // Uses the previous iterator and the current one to constuct the string_view.
                this->parts[i] = std::string_view(_oldIt, _it);
            }
            for (std::size_t i = 0; i < suffixes.size(); ++i) {
                std::string_view _s = suffixes[i];

                std::string::iterator _oldIt = _it;
                _it = std::copy(_s.begin(), _s.end(), _it);

                // Uses the previous iterator and the current one to constuct the string_view.
                this->suffixes[i] = std::string_view(_oldIt, _it);
            }
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
            return hasSpaceAfter;
        }

        bool isPunct(char c) {
            return c != '_' && std::ispunct(c, std::locale::classic());
        }
        bool isDigit(char c) {
            return std::isdigit(c, std::locale::classic());
        }
        bool isSpace(char c) {
            return std::isspace(c, std::locale::classic());
        }
        bool isControl(char c) {
            return std::iscntrl(c, std::locale::classic());
        }
        std::deque<Token> tokenise(std::string_view str) {
            std::deque<Token> _tokens;
            return std::move(_tokens);
        }
    }
}