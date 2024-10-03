#include "string_twine.h"
#include <iostream>

int main() {
    StringTwine<char> twine;

    COWString<char> str1("Hello, ");
    COWString<char> str2("World!");
    COWString<char> str3(" How are you?");

    twine.append(str1);
    twine.append(str2);
    twine.prepend(str3);

    COWString<char> result = twine.str();
    std::cout << "Concatenated Result: " << result.data() << std::endl;

    return 0;
}
