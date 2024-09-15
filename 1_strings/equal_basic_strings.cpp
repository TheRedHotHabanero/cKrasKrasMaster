/*
Напишите для std::basic_string сравнение (оператор ==).
1. сделаете ли вы этот оператор методом класса или собственной функцией? (придется классом видимо)
2. как должны сравниваться строки с одинаковым CharT, но разными Traits? (никак)
3. а если у них одинаковые CharT и Traits, но разные аллокаторы? (фиолетово, сравниваем содержимое)
4.
    "hello" == str
    str == "hello" (using previous)
    str2 == str1
    "hello" == "aaaaa" -> std::strcmp
*/

#include <iostream>
#include <string>
#include <cstring>

// "hello" == str
namespace help {

    template<class CharT, class Traits, class Allocator1>
    bool operator==(const CharT* lhs,
                    const std::basic_string<CharT, Traits, Allocator1>& rhs) noexcept
    {
        std::cout << "my operator" << std::endl;
        return rhs.compare(lhs) == 0;
    }

    // str == "hello"
    template<class CharT, class Traits, class Allocator2>
    bool operator==(const std::basic_string<CharT, Traits, Allocator2>& lhs,
                    const CharT* rhs) noexcept
    {
        std::cout << "my operator" << std::endl;
        return operator==(rhs, lhs);
    }

    // str2 == str1 с учетом возможности разных аллокаторов
    template<class CharT, class Traits, class Allocator1, class Allocator2>
    bool operator==(const std::basic_string<CharT, Traits, Allocator1>& lhs,
                    const std::basic_string<CharT, Traits, Allocator2>& rhs) noexcept
    {
        std::cout << "my operator" << std::endl;
        return lhs.size() == rhs.size() &&
               Traits::compare(lhs.data(), rhs.data(), lhs.size()) == 0;
    }

}

int main() {
    // Одинаковые CharT, Traits, Allocator
    std::basic_string<char> str1 = "hello";
    std::basic_string<char> str2 = "hello";
    if (help::operator==(str1, str2)) {
        std::cout << "Equal 1" << std::endl;
        std::cout << "-------" << std::endl;
    }

    // Сравнение std::basic_string с С-строкой (2я и 1я перегрука)
    if (help::operator==(str1, "hello")) { // ambiguous overload for ‘operator==’
        std::cout << "Equal 2" << std::endl;
        std::cout << "-------" << std::endl;
    }
    if (help::operator==("hello", str1)) {
        std::cout << "Equal 3" << std::endl;
        std::cout << "-------" << std::endl;
    }

    // Сравнение двух С-строк (просто чтобы было)
    if (std::strcmp("hello", "hello") == 0) {
        std::cout << "Equal 4" << std::endl;
        std::cout << "-------" << std::endl;
    }

    // Сравнение с разными Traits
    // CTE без кастомных Traits

    // Сравнение с разными Allocator`ами
    std::basic_string<char, std::char_traits<char>, std::allocator<char>> str_alloc1 = "hello";
    std::basic_string<char, std::char_traits<char>, std::allocator<int>> str_alloc2 = "hello";

    if (help::operator==(str_alloc1, str_alloc2)) {
        std::cout << "Equal 5" << std::endl;
        std::cout << "-------" << std::endl;
    }

    return 0;
}