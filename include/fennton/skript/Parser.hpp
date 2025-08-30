#include <deque>
#include <vector>
#include <string>
#include <variant>
#include <cstdint>

namespace Fennton::Skript {
    namespace Tokeniser {
        class Name {
        private:
            std::string spelling;
        public:
            bool operator==(Name const& other) const;
            bool operator!=(Name const& other) const;
            // Returns the token's spelling, not exactly equal to how it was spelled in the 
            // source, but which generates the same token if retokenised.
            std::string GetSpelling() const;
        };
        class Number {
        private:
            // Contiguous string containing the number's parts and suffixes.
            std::string storage;
            // Vector instead of list because it allows random indexing and it is going 
            // to handle the memory allocations and deallocations.
            std::vector<std::string_view> parts;
            // The number's suffixes.
            std::vector<std::string_view> suffixes;
            // The number's base.
            std::int32_t base;
        public:
            // Constructor with explicit storage.
            Number(
                std::string const& storage,
                std::vector<std::string_view> const& parts,
                std::vector<std::string_view> const& suffixes,
                std::int32_t base
            );
            // Construtor with the storage string built automatically.
            Number(
                std::vector<std::string_view> const& parts,
                std::vector<std::string_view> const& suffixes,
                std::int32_t base
            );
            bool operator==(Number const& other) const;
            bool operator!=(Number const& other) const;
            // Returns the token's spelling, not exactly equal to how it was spelled in the 
            // source, but which generates the same token if retokenised.
            std::string GetSpelling() const;
            std::vector<std::string_view> const& GetParts() const;
            std::vector<std::string_view> const& GetSuffixes() const;
            std::int32_t GetBase() const;
        };
        class String {
        public:
            bool operator==(String const& other) const;
            bool operator!=(String const& other) const;
            // Returns the token's spelling, not exactly equal to how it was spelled in the 
            // source, but which generates the same token if retokenised.
            std::string GetSpelling() const;
        };
        class Punct {
        public:
            bool operator==(Punct const& other) const;
            bool operator!=(Punct const& other) const;
            // Returns the token's spelling, not exactly equal to how it was spelled in the 
            // source, but which generates the same token if retokenised.
            std::string GetSpelling() const;
        };
        class Token {
        public:
            using VariantType = std::variant<Name, Number, String, Punct>;
        private:
            VariantType var;
            bool hasSpaceAfter;

            template<typename T> Token(T const& innerVal) {
                var = innerVal;
            }

            Token(Token const&) = delete;
            Token(Token&&) = delete;
            Token& operator=(Token const& other) = delete;
            Token& operator=(Token&& other) = delete;
        public:
            static constexpr std::int32_t spaceAfterBit = 0;
            // Compares two tokens for equality.
            bool operator==(Token const& other) const {
                return var == other.var;
            }
            // Compares two tokens for inequality.
            bool operator!=(Token const& other) const {
                return !(*this == other);
            }
            // Returns the token's spelling, not exactly equal to how it was spelled in the 
            // source, but which generates the same token if retokenised.
            std::string GetSpelling() const;
            // Returns true if there is a space between this token and the next and false if 
            // there is either no space or no token.
            constexpr bool HasSpaceAfter();
        };
        // Returns true if the character is in the !"#$%&'()*+,-./:;<=>?@[\]^`{|}~ set (the set 
        // of punctuation defined by the classic C locale, minus the `_` character), else returns 
        // false.
        bool isPunct(char c);
        // Returns true if the character is a decimal digit (in the 0123456789 set), else returns 
        // false.
        bool isDigit(char c);
        // Returns true if the character is whitespace, in the set: 0x20 (' ', space), 
        // 0x0c ('\f', form feed), 0x0a ('\n', line feed), 0x0d ('\r', carriage return), 
        // 0x09 ('\t', horizontal tab), 0x0b ('\v', vertical tab). Else, returns false.
        bool isSpace(char c);
        // Returns true if the character is a control character (the set include some 
        // character classified as whitespace), else returns false.
        bool isControl(char c);
        // Tokenises the string into a deque and returns it. The tokens do not depend on the original string, 
        // so it is free to deallocate it.
        std::deque<Tokeniser::Token> tokenise(std::string_view str);
    };
}