#include <fennton/utils/Text.hpp>

#include <sstream>

namespace Fennton::Text {
    char escapeSimpleChar(char c) {
        switch (c) {
            case '\\':
                return '\\';
            case '\'':
                return '\'';
            case '\"':
                return '"';
            case '\?':
            	return '?';
            case '\a':
            	return 'a';
            case '\b':
            	return 'b';
            case '\f':
            	return 'f';
            case '\n':
            	return 'n';
            case '\r':
            	return 'r';
            case '\t':
            	return 't';
            case '\v':
            	return 'v';
            
            // Single-digit octal escaping.
            case '\0':
                return '0';
            case '\1':
                return '1';
            case '\2':
                return '2';
            case '\3':
                return '3';
            case '\4':
                return '4';
            case '\5':
                return '5';
            case '\6':
                return '6';
            // Not enabled because '\a' is the same as '\7'.
            // case '\7':
            //     return '7';

            // Returns the null character to show the character has no escape sequence.
            default:
                return '\0';
        }
    }
    std::string escape(std::string const& str) {
        std::stringstream _ss;
        for (char c : str) {
            char _escaped = escapeSimpleChar(c);
            if (_escaped != '\0') {
                _ss << '\\' << _escaped;
            } else {
                _ss << c;
            }
            // TODO: 1-3 octal sequences.
        }
        return _ss.str();
    }
    std::string quote(std::string const& str) {
        return "\"" + escape(str) + "\"";
    }
}