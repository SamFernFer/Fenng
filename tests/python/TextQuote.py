# Command: python tests/python/TextQuote.py > tests/include/fennton/utils/TextQuoteGenerated.hpp

import random

# Generates random test cases with C++ strings with characters needing escaping inside.

_seqs = [
    '\\', "'", '"',
    '?', 'a', 'b', 'f', 'n', 'r', 't', 'v',
    '0', '1', '2', '3', '4', '5', '6',
    # Unused, because it's the same as '\a'.
    # '7'
]

random.randrange(0, len(_seqs))

print("// Single control character.\n")
for s in _seqs:
    print(f'_case(R"(\{s})"s, "\{s}"s);')
print()

print("// Multiple repeated control characters (2 to 4).\n")

for i in range(2,5):
    print(f"// - {i}:")
    for s in _seqs:
        _esc = "\\" + s
        print(f'_case(R"({_esc * i})"s, "{_esc * i}"s);')
    print()

print("// (TODO) All combinations of control characters (2 to 4).\n")