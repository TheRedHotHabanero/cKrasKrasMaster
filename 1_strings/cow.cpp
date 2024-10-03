#include "cow.h"
#include <gtest/gtest.h>

// Тесты для COWString с char -------------------------------------------------
class COWStringCharTest : public ::testing::Test {
  protected:
    COWString<char> cowStr;

    void SetUp() override { cowStr = COWString<char>("Hello, World!"); }
};

TEST(COWStringTest, DefaultConstructor) {
    COWString<char> str;
    EXPECT_EQ(str.find_substr(""), -1);
}

TEST_F(COWStringCharTest, CopyConstructor) {
    COWString<char> strCopy = cowStr;
    EXPECT_EQ(strCopy.find_substr("Hello"), 0);
}

TEST_F(COWStringCharTest, MoveConstructor) {
    COWString<char> strMove = std::move(cowStr);
    EXPECT_EQ(strMove.find_substr("World"), 7);
}

TEST_F(COWStringCharTest, CopyAssignmentOperator) {
    COWString<char> strCopy;
    strCopy = cowStr;
    EXPECT_EQ(strCopy.find_substr("Hello"), 0);
}

TEST_F(COWStringCharTest, MoveAssignmentOperator) {
    COWString<char> strMove;
    strMove = std::move(cowStr);
    EXPECT_EQ(strMove.find_substr("World"), 7);
}

TEST_F(COWStringCharTest, Tokenization) {
    auto tokens = cowStr.tokenize(", "); // Токенизация по запятой
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0], "Hello");
    EXPECT_EQ(tokens[1], "World!");
}

TEST_F(COWStringCharTest, FindSubstr) {
    EXPECT_EQ(cowStr.find_substr("World"), 7);
    EXPECT_EQ(cowStr.find_substr("Hello"), 0);
    EXPECT_EQ(cowStr.find_substr("NotFound"), -1);
}

// Тесты для wchar_t
// ---------------------------------------------------------------------------
class COWStringWCharTest : public ::testing::Test {
  protected:
    COWString<wchar_t> cowStr;

    void SetUp() override { cowStr = COWString<wchar_t>(L"Hello, World!"); }
};

TEST_F(COWStringWCharTest, TokenizationW) {
    auto tokens = cowStr.tokenize(L", "); // Токенизация по запятой
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0], L"Hello");
    EXPECT_EQ(tokens[1], L"World!");
}

TEST_F(COWStringWCharTest, FindSubstrW) {
    EXPECT_EQ(cowStr.find_substr(L"World"), 7);
    EXPECT_EQ(cowStr.find_substr(L"Hello"), 0);
    EXPECT_EQ(cowStr.find_substr(L"NotFound"), -1);
}

// More tokenization tests
TEST(TokenizeSimple, COWStringTest) {
    COWString<char> str("one,two,three");
    std::vector<std::string> expected_tokens = {"one", "two", "three"};
    std::vector<std::string> tokens = str.tokenize(",");
    EXPECT_EQ(tokens.size(), expected_tokens.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i], expected_tokens[i]);
    }
}

// Тест токенизации строки с несколькими разделителями
TEST(TokenizeMultipleDelimiters, COWStringTest) {
    COWString<char> str("one,two;three");
    std::vector<std::string> expected_tokens = {"one", "two", "three"};
    std::vector<std::string> tokens = str.tokenize(",;");
    EXPECT_EQ(tokens.size(), expected_tokens.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i], expected_tokens[i]);
    }
}

// Тест токенизации строки с пробелами как разделителями
TEST(TokenizeWithSpaces, COWStringTest) {
    COWString<char> str("one two three");
    std::vector<std::string> expected_tokens = {"one", "two", "three"};
    std::vector<std::string> tokens = str.tokenize(" ");
    EXPECT_EQ(tokens.size(), expected_tokens.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i], expected_tokens[i]);
    }
}

// Тест токенизации строки с широкими символами 'wchar_t'
TEST(TokenizeWChar, COWStringWCharTest) {
    COWString<wchar_t> str(L"один,два,три");
    std::vector<std::wstring> expected_tokens = {L"один", L"два", L"три"};
    std::vector<std::wstring> tokens = str.tokenize(L",");
    EXPECT_EQ(tokens.size(), expected_tokens.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i], expected_tokens[i]);
    }
}

// Тест токенизации строки с несколькими разделителями для wchar_t
TEST(TokenizeMultipleDelimitersWChar, COWStringWCharTest) {
    COWString<wchar_t> str(L"один,два;три");
    std::vector<std::wstring> expected_tokens = {L"один", L"два", L"три"};
    std::vector<std::wstring> tokens = str.tokenize(L",;");
    EXPECT_EQ(tokens.size(), expected_tokens.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i], expected_tokens[i]);
    }
}

// Тест токенизации строки с пробелами для wchar_t
TEST(TokenizeWithSpacesWChar, COWStringWCharTest) {
    COWString<wchar_t> str(L"один два три");
    std::vector<std::wstring> expected_tokens = {L"один", L"два", L"три"};
    std::vector<std::wstring> tokens = str.tokenize(L" ");
    EXPECT_EQ(tokens.size(), expected_tokens.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i], expected_tokens[i]);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
