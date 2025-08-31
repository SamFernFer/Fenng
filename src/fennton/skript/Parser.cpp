#include <fennton/skript/Parser.hpp>
#include <utility>
#include <type_traits>
#include <stdexcept>
#include <utility>
#include <locale>
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
            std::vector<std::pair<std::size_t, std::size_t>>
                // Vector of index ranges on the storage string, so that it is possible to 
                // generate the vector of parts later.
                _partsRanges(parts.size()),
                // Vector of index ranges on the storage string for the suffixes, for the 
                // same reason.
                _suffixesRanges(suffixes.size())
            ;

            std::size_t _currIndexOnStorage = 0;
            // storage has already been default-initialised, so it is safe to apped to it.
            // Also, the number of parts is usually small, so there is not need for a 
            // stringstream.
            for (std::size_t i = 0; i < parts.size(); ++i) {
                std::string_view _s = parts[i];
                std::pair<std::size_t, std::size_t>& _range = _partsRanges[i];
                
                _range.first = _currIndexOnStorage;
                _currIndexOnStorage += _s.size();
                _range.second = _currIndexOnStorage;
                // Appends each part to storage.
                storage.append(s);
            }
            this->parts = parts;
            this->suffixes = suffixes;
            this->base = base;
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