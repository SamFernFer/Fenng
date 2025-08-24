#include <deque>
#include <string>
#include <variant>
#include <cstdint>

namespace Fennton::Skript {
    namespace Tokeniser {
        class Token {
            std::variant<std::string_view, std::string> spelling;
        public:
            bool operator==(Token const& other) const {
                return false;
            }
        };
        class Name {
        };
        class Number {
            // Vector instead of list because it allows random indexing and instead 
            // of deque because it's more memory-efficient.
            std::vector<std::uint64_t> parts;
        };
        class String {
        };
        class Punct {
        };
    };
    std::deque<Tokeniser::Token> tokenise(std::string_view str);
}