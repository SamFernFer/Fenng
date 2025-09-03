#include <deque>
#include <vector>
#include <string>
#include <optional>
#include <variant>
#include <stdexcept>
#include <cstdint>

namespace Fennton::Skript {
    namespace Tokeniser {
        class Token;

        using TokenResult = std::pair<std::string_view::const_iterator, Token>;

        // Returned by the isDigit16 function, to make tokenisation error messages more 
        // helpful by explaining that uppercase letters are disallowed.
        enum class HexError {
            True, // It is a hexadecimal digit.
            False, // It is NOT a hexadecimal digit.
            Uppercase // It is an uppercase hexadecimal digit, so it is invalid.
        };
 
        class Exception : public std::runtime_error {
        public:
            using std::runtime_error::runtime_error;
        };
        class NotImplementedException : public Exception {
        public:
            using Exception::Exception;
        };

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
            // Moves the fields from the other object into this.
            void moveFrom(Number&& other);
        public:
            Number(Number const&) = delete;
            Number(Number&& other);
            // Constructor with explicit storage.
            Number(
                std::string&& storage,
                std::vector<std::string_view>&& parts,
                std::vector<std::string_view>&& suffixes,
                std::int32_t base
            );
            // Construtor with the storage string built automatically.
            Number(
                std::vector<std::string_view> const& parts,
                std::vector<std::string_view> const& suffixes,
                std::int32_t base
            );
            Number& operator=(Number const&) = delete;
            Number& operator=(Number&& other);
            bool operator==(Number const& other) const;
            bool operator!=(Number const& other) const;
            // Parses the largest base-2 number token in the range [start, end), throwing if 
            // unexpected characters or character sequences are present before the number's 
            // end. Returns a pair containing the iterator after the number's end and 
            // the number token.
            static TokenResult parseBase2(
                std::string_view::const_iterator start,
                std::string_view::const_iterator end
            );
            // Same as parseBase2, but for base-8.
            static TokenResult parseBase8(
                std::string_view::const_iterator start,
                std::string_view::const_iterator end
            );
            // Same as parseBase2, but for base-10.
            static TokenResult parseBase10(
                std::string_view::const_iterator start,
                std::string_view::const_iterator end
            );
            // Same as parseBase2, but for base-16.
            static TokenResult parseBase16(
                std::string_view::const_iterator start,
                std::string_view::const_iterator end
            );
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

            // Moves the fields from another Token object into this.
            void moveFrom(Token&&);
        public:
            // static constexpr std::int32_t spaceAfterBit = 0;

            Token(Token const&) = delete;
            Token(Token&&);
            Token() = default;
            template<typename T> Token(T&& innerVal, bool hasSpaceAfter) {
                var = std::move(innerVal);
                this->hasSpaceAfter = hasSpaceAfter;
            }
            Token& operator=(Token const& other) = delete;
            Token& operator=(Token&& other);
            // Compares two tokens for equality.
            bool operator==(Token const& other) const;
            // Compares two tokens for inequality.
            bool operator!=(Token const& other) const;
            // Returns the token's spelling, not exactly equal to how it was spelled in the 
            // source, but which generates the same token if retokenised.
            std::string GetSpelling() const;
            // Returns true if there is a space between this token and the next and false if 
            // there is either no space or no token.
            bool HasSpaceAfter() const;
            // Sets the value of the hasSpaceAfter field.
            void HasSpaceAfter(bool hasSpaceAfter);
        };
        // Returns true if the character is in the !"#$%&'()*+,-./:;<=>?@[\]^`{|}~ set (the set 
        // of punctuation defined by the classic C locale, minus the `_` character), else returns 
        // false.
        bool isPunct(char c);
        // Returns true if the character is a binary digit (0 or 1), else returns false.
        bool isDigit2(char c);
        // Returns true if the character is an octal digit (in the 01234567 set), else returns 
        // false.
        bool isDigit8(char c);
        // Returns true if the character is a decimal digit (in the 0123456789 set), else 
        // returns false.
        bool isDigit10(char c);
        // Returns HexError::True if the character is a lowercase hexadecimal digit (in the
        // 0123456789abcdef set), HexError::Uppercase if the character is an uppercase 
        // hexadecimal digit (in the ABCDEF set) and false otherwise.
        HexError isDigit16(char c);
        // Returns true if the character is whitespace, in the set: 0x20 (' ', space), 
        // 0x0c ('\f', form feed), 0x0a ('\n', line feed), 0x0d ('\r', carriage return), 
        // 0x09 ('\t', horizontal tab), 0x0b ('\v', vertical tab). Else, returns false.
        bool isSpace(char c);
        // Returns true if the character is a control character (the set include some 
        // character classified as whitespace), else returns false.
        bool isControl(char c);
        // Consumes whitespace, single-line comments and multi-line comments.
        std::string_view::const_iterator consumeSpace(
            std::string_view::const_iterator start,
            std::string_view::const_iterator end
        );
        // Tokenises the next token from the start iterator until before the end iterator.
        TokenResult tokeniseNext(
            std::string_view::const_iterator start,
            std::string_view::const_iterator end
        );
        // Tokenises the string into a deque and returns it. The tokens do not depend on the original string, 
        // so it is free to deallocate it.
        std::deque<Tokeniser::Token> tokenise(std::string_view str);
    };
}