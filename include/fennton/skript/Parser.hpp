#include <deque>
#include <string>
#include <variant>
#include <cstdint>

namespace Fennton::Skript {
    namespace Tokeniser {
        class Name {
        };
        class Number {
            // Contiguous string containing the number's parts and suffixes.
            std::string str;
            // Vector instead of list because it allows random indexing and it is going 
            // to handle the memory allocations and deallocations.
            std::vector<std::string_view> parts;
            // The number's suffixes.
            std::vector<std::string_view> suffixes;
            // The number's base.
            std::int32_t base;
        public:
            std::int32_t GetBase() const;
            std::vector<std::string_view> const& GetParts() const;
            std::vector<std::string_view> const& GetSuffixes() const;
        };
        class String {
        };
        class Punct {
        };
        class Token {
            std::variant<Name, Number, String, Punct> var;
        public:
            Token();
            bool operator==(Token const& other) const {
                return var == other.var;
            }
            // Returns the token's spelling.
            std::string const& GetSpelling();
        };
    };
    std::deque<Tokeniser::Token> tokenise(std::string_view str);
}