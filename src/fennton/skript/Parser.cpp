#include <fennton/skript/Parser.hpp>
#include <utility>
#include <algorithm>
#include <format>
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
            throw NotImplementedException("Not implemented yet.");
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
                && (base == other.base)
            ;
        }
        bool Number::operator!=(Number const& other) const {
            return !(*this == other);
        }
        std::string Number::GetSpelling() const {
            // Base size.
            std::size_t _size = storage.size();
            // If there is more than one part, then reserves space for the radixes.
            if (parts.size() > 1) { _size += parts.size() - 1; }
            // If there is more than one suffix, then reserves space for the 
            // suffix separators.
            if (suffixes.size() > 1) { _size += suffixes.size() - 1; }
            // Increases the size based on the base and its necessary prefixes or delimiters.
            switch (base) {
                case 2:
                    // Space for the base prefix ("0b").
                    _size += 2;
                    break;
                case 8:
                    // Space for the base prefix ("0").
                    _size += 1;
                    break;
                case 10:
                    // No prefix.
                    break;
                case 16:
                    // Space for the base prefix ("0x") and for the radix followed by a 
                    // suffix/number separator (".'"), necessary before the suffix list, 
                    // as a base-16 number includes letters.
                    _size += 4;
                    break;
                default:
                    throw std::runtime_error(std::format(
                        "Number::GetSpelling: Base {} is unsupported.", base
                    ));
            }
            // Allocates the spelling string a single time with the correct size.
            std::string _spelling = std::string(_size, '#');
            // Iterator to the beginning of the spelling string.
            std::string::iterator _spellingIt = _spelling.begin();
            switch (base) {
                // Prefix "0b".
                case 2:
                    *(_spellingIt++) = '0';
                    *(_spellingIt++) = 'b';
                    break;
                // Prefix "0".
                case 8:
                    *(_spellingIt++) = '0';
                    break;
                // Prefix "0x".
                case 16:
                    *(_spellingIt++) = '0';
                    *(_spellingIt++) = 'x';
                    break;
                // No need for a default case, as unsupported bases have already been handled.
            }
            // Writes the number's parts.
            if (parts.size() > 0) {
                // Iterator to the beginning of the parts vector.
                auto _partsIt = parts.begin();
                // Copies the first part to the spelling string and updates the spelling 
                // iterator.
                _spellingIt = std::copy(_partsIt->begin(), _partsIt->end(), _spellingIt);
                // Moves to the second part. The loop automatically prevents using a non-
                // -existent second part.
                ++_partsIt;
                for (; _partsIt != parts.cend(); ++_partsIt) {
                    // Writes the radix and increments the spelling iterator.
                    *(_spellingIt++) = '.';
                    // Writes the part and updates the spelling iterator.
                    _spellingIt = std::copy(_partsIt->begin(), _partsIt->end(), _spellingIt);
                }
            }
            // Writes the number's suffixes.
            if (suffixes.size() > 0) {
                // Base-16 includes letters, so an unambiguous separator is necessary.
                if (base == 16) {
                    *(_spellingIt++) = '.';
                    *(_spellingIt++) = '\'';
                }
                // Iterator to the beginning of the suffixes vector.
                auto _suffixesIt = suffixes.begin();
                // Copies the first suffix to the spelling string and updates the 
                // spelling iterator.
                _spellingIt = std::copy(_suffixesIt->begin(), _suffixesIt->end(), _spellingIt);
                // Moves to the second suffix. The loop automatically prevents using a non-
                // -existent second suffix.
                ++_suffixesIt;
                for (; _suffixesIt != suffixes.end(); ++_suffixesIt) {
                    // Writes the suffix separator and increments the spelling iterator.
                    *(_spellingIt++) = '\'';
                    // Writes the part and updates the spelling iterator.
                    _spellingIt = std::copy(_suffixesIt->begin(), _suffixesIt->end(), _spellingIt);
                }
            }
            return std::move(_spelling);
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
            throw NotImplementedException("Not implemented yet.");
        }

        bool Punct::operator==(Punct const& other) const {
            return false;
        }
        bool Punct::operator!=(Punct const& other) const {
            return !(*this == other);
        }
        std::string Punct::GetSpelling() const {
            throw NotImplementedException("Not implemented yet.");
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
        std::pair<Token, NextMode> tokeniseNext(
            std::string_view str,
            std::string_view::const_iterator it
        ) {
            if (i < str.size()) {
                switch (str[i]) {
                    // Base 2, 8, 10 or 16.
                    case '0':
                        ++i;
                        if (i < str.size()) {
                            switch (std[i]) {
                                // Binary (`0b`).
                                case 'b':
                                    return parseBase2();
                                // Octal (`0o`).
                                case 'o':
                                    return parseBase8();
                                // Hexadecimal (`0x`).
                                case 'x':
                                    return parseBase16();
                                // Decimal (no prefix).
                                case '0':
                                case '1':
                                case '2':
                                case '3':
                                case '4':
                                case '5':
                                case '6':
                                case '7':
                                case '8':
                                case '9':
                                    return parseBase10();
                            }
                        } else {
                            return { NextMode::TokenAndEOF, parseBase10() };
                        }
                    // Base 10 number.
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        return parseBase10();
                }
            }
        }
        std::deque<Token> tokenise(std::string_view str) {
            std::deque<Token> _tokens;

            std::string_view::const_iterator _it = str.begin();
            for (;;) {
                // Consumes whitespace and comments before trying to emit a token.
                if (!consumeSpace(str, _it)) {
                    // There might be a token - and if there is, it might be right before 
                    // EOF - and there might be EOF without a token.
                    std::pair<NextMode, Token> _maybeNextToken = tokeniseNext(str, _it);
                    switch (_maybeNextToken.first) {
                        case NextMode::Token:
                            _tokens.emplace_back(std::move(_maybeNextToken.second));
                            break;
                        case NextMode::TokenAndEOF:
                            _tokens.emplace_back(std::move(_maybeNextToken.second));
                            [[fallthrough]];
                        case NextMode::Eof:
                            // If the EOF has been reached, then returns the deque.
                            return std::move(_tokens);
                    }
                }
            }
        }
    }
}