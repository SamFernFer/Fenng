// Single control character.

_case(R"(\\)"s, "\\"s);
_case(R"(\')"s, "\'"s);
_case(R"(\")"s, "\""s);
_case(R"(\?)"s, "\?"s);
_case(R"(\a)"s, "\a"s);
_case(R"(\b)"s, "\b"s);
_case(R"(\f)"s, "\f"s);
_case(R"(\n)"s, "\n"s);
_case(R"(\r)"s, "\r"s);
_case(R"(\t)"s, "\t"s);
_case(R"(\v)"s, "\v"s);
_case(R"(\0)"s, "\0"s);
_case(R"(\1)"s, "\1"s);
_case(R"(\2)"s, "\2"s);
_case(R"(\3)"s, "\3"s);
_case(R"(\4)"s, "\4"s);
_case(R"(\5)"s, "\5"s);
_case(R"(\6)"s, "\6"s);

// Multiple repeated control characters (2 to 4).

// - 2:
_case(R"(\\\\)"s, "\\\\"s);
_case(R"(\'\')"s, "\'\'"s);
_case(R"(\"\")"s, "\"\""s);
_case(R"(\?\?)"s, "\?\?"s);
_case(R"(\a\a)"s, "\a\a"s);
_case(R"(\b\b)"s, "\b\b"s);
_case(R"(\f\f)"s, "\f\f"s);
_case(R"(\n\n)"s, "\n\n"s);
_case(R"(\r\r)"s, "\r\r"s);
_case(R"(\t\t)"s, "\t\t"s);
_case(R"(\v\v)"s, "\v\v"s);
_case(R"(\0\0)"s, "\0\0"s);
_case(R"(\1\1)"s, "\1\1"s);
_case(R"(\2\2)"s, "\2\2"s);
_case(R"(\3\3)"s, "\3\3"s);
_case(R"(\4\4)"s, "\4\4"s);
_case(R"(\5\5)"s, "\5\5"s);
_case(R"(\6\6)"s, "\6\6"s);

// - 3:
_case(R"(\\\\\\)"s, "\\\\\\"s);
_case(R"(\'\'\')"s, "\'\'\'"s);
_case(R"(\"\"\")"s, "\"\"\""s);
_case(R"(\?\?\?)"s, "\?\?\?"s);
_case(R"(\a\a\a)"s, "\a\a\a"s);
_case(R"(\b\b\b)"s, "\b\b\b"s);
_case(R"(\f\f\f)"s, "\f\f\f"s);
_case(R"(\n\n\n)"s, "\n\n\n"s);
_case(R"(\r\r\r)"s, "\r\r\r"s);
_case(R"(\t\t\t)"s, "\t\t\t"s);
_case(R"(\v\v\v)"s, "\v\v\v"s);
_case(R"(\0\0\0)"s, "\0\0\0"s);
_case(R"(\1\1\1)"s, "\1\1\1"s);
_case(R"(\2\2\2)"s, "\2\2\2"s);
_case(R"(\3\3\3)"s, "\3\3\3"s);
_case(R"(\4\4\4)"s, "\4\4\4"s);
_case(R"(\5\5\5)"s, "\5\5\5"s);
_case(R"(\6\6\6)"s, "\6\6\6"s);

// - 4:
_case(R"(\\\\\\\\)"s, "\\\\\\\\"s);
_case(R"(\'\'\'\')"s, "\'\'\'\'"s);
_case(R"(\"\"\"\")"s, "\"\"\"\""s);
_case(R"(\?\?\?\?)"s, "\?\?\?\?"s);
_case(R"(\a\a\a\a)"s, "\a\a\a\a"s);
_case(R"(\b\b\b\b)"s, "\b\b\b\b"s);
_case(R"(\f\f\f\f)"s, "\f\f\f\f"s);
_case(R"(\n\n\n\n)"s, "\n\n\n\n"s);
_case(R"(\r\r\r\r)"s, "\r\r\r\r"s);
_case(R"(\t\t\t\t)"s, "\t\t\t\t"s);
_case(R"(\v\v\v\v)"s, "\v\v\v\v"s);
_case(R"(\0\0\0\0)"s, "\0\0\0\0"s);
_case(R"(\1\1\1\1)"s, "\1\1\1\1"s);
_case(R"(\2\2\2\2)"s, "\2\2\2\2"s);
_case(R"(\3\3\3\3)"s, "\3\3\3\3"s);
_case(R"(\4\4\4\4)"s, "\4\4\4\4"s);
_case(R"(\5\5\5\5)"s, "\5\5\5\5"s);
_case(R"(\6\6\6\6)"s, "\6\6\6\6"s);

// (TODO) All combinations of control characters (2 to 4).

