#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <vector>

template <typename CharT, typename Traits = std::char_traits<CharT>>
class COWString {
  private:
    struct StringData {
        std::unique_ptr<CharT[]> data;
        size_t ref_count;
        size_t capacity;

        StringData(const CharT *str) {
            size_t len = Traits::length(str);
            data = std::make_unique<CharT[]>(len + 1);
            Traits::copy(data.get(), str, len);
            data[len] = '\0';
            ref_count = 1;
            capacity = len + 1;
        }

        ~StringData() = default;
    };

    StringData *str_data;

    void make_unique() {
        if (str_data->ref_count > 1) {
            str_data->ref_count--;
            str_data = new StringData(str_data->data.get());
        }
    }

  public:
    COWString()
        : str_data(new StringData(reinterpret_cast<const CharT *>(""))) {}
    COWString(const CharT *str) : str_data(new StringData(str)) {}

    // Copy constructor
    COWString(const COWString &other) : str_data(other.str_data) {
        str_data->ref_count++;
    }

    // Move constructor
    COWString(COWString &&other) noexcept : str_data(other.str_data) {
        other.str_data = nullptr;
    }

    // Copy assignment
    COWString &operator=(const COWString &other) {
        if (this != &other) {
            if (--str_data->ref_count == 0) {
                delete str_data;
            }
            str_data = other.str_data;
            str_data->ref_count++;
        }
        return *this;
    }

    // Move assignment
    COWString &operator=(COWString &&other) noexcept {
        if (this != &other) {
            if (--str_data->ref_count == 0) {
                delete str_data;
            }
            str_data = other.str_data;
            other.str_data = nullptr;
        }
        return *this;
    }

    ~COWString() {
        if (str_data && --str_data->ref_count == 0) {
            delete str_data;
        }
    }

    bool operator<(const COWString &other) const {
        return std::lexicographical_compare(
            this->data(), this->data() + this->size(), other.data(),
            other.data() + other.size());
    }

    bool operator>(const COWString &other) const { return other < *this; }

    bool operator==(const COWString &other) const {
        return this->size() == other.size() &&
               std::equal(this->data(), this->data() + this->size(),
                          other.data());
    }

    COWString operator+(const COWString &other) const {
        size_t new_size = this->size() + other.size();
        std::unique_ptr<CharT[]> new_data =
            std::make_unique<CharT[]>(new_size + 1);

        Traits::copy(new_data.get(), this->data(), this->size());
        Traits::copy(new_data.get() + this->size(), other.data(), other.size());
        new_data[new_size] = '\0';

        return COWString(
            new_data.get());
    }

    size_t find_substr(const CharT *substr) const {
        if (!str_data || !substr)
            return std::string::npos;

        const CharT *pos =
            std::search(this->data(), this->data() + this->size(), substr,
                        substr + Traits::length(substr));

        return (pos != this->data() + this->size()) ? pos - this->data()
                                                    : std::string::npos;
    }

    size_t size() const { return Traits::length(str_data->data.get()); }

    const CharT *data() const { return str_data->data.get(); }

    std::vector<std::basic_string<CharT>>
    tokenize(const CharT *delimiters = L" ") const {
        std::vector<std::basic_string<CharT>> tokens;

        if (Traits::length(str_data->data.get()) == 0) {
            return tokens;
        }

        auto copy =
            std::make_unique<CharT[]>(Traits::length(str_data->data.get()) + 1);
        Traits::copy(copy.get(), str_data->data.get(),
                     Traits::length(str_data->data.get()) + 1);

        CharT *current = copy.get();
        CharT *next_token = nullptr;

        while (*current) {
            while (*current &&
                   Traits::find(delimiters, Traits::length(delimiters),
                                *current) != nullptr) {
                ++current;
            }
            if (*current == '\0') {
                break;
            }
            next_token = current;
            while (*current &&
                   Traits::find(delimiters, Traits::length(delimiters),
                                *current) == nullptr) {
                ++current;
            }
            tokens.emplace_back(next_token, current - next_token);
        }

        return tokens;
    }
};
