#pragma once

#include <algorithm>
#include <array>
#include <stdexcept>
#include <string_view>

namespace gnev {

template <std::size_t Size>
struct CtString {
private:
    template <std::size_t S>
    static consteval std::size_t initLength(const char (&str)[S]) {
        auto term = std::find(std::begin(str), std::end(str), '\0');
        if (term == std::end(str)) {
            throw std::logic_error("CtString supports nullterm strings only");
        }
        return std::min<std::size_t>(std::min(Size, S),
                                     std::distance(std::begin(str), term + 1));
    }

    template <std::size_t S>
    static consteval std::size_t initLength(const std::array<char, S>& str) {
        auto term = std::find(std::begin(str), std::end(str), '\0');
        if (term == std::end(str)) {
            throw std::logic_error("CtString supports nullterm strings only");
        }
        return std::min<std::size_t>(std::min(Size, S),
                                     std::distance(std::begin(str), term) + 1);
    }

    template <std::size_t S>
    static consteval std::array<char, Size> initArray(const char (&str)[S]) {
        std::array<char, Size> res = {};
        std::copy(std::begin(str), std::end(str), res.begin());
        return res;
    }

    template <std::size_t S>
    static consteval std::array<char, Size> initArray(const std::array<char, S>& str) {
        std::array<char, Size> res = {};
        std::copy(std::begin(str), std::end(str), res.begin());
        return res;
    }

    static consteval std::array<char, Size> initArray(const std::string_view& str) {
        std::array<char, Size> res = {};
        std::fill(res.begin(), res.end(), '\0');
        std::copy(str.data(), str.data() + std::min(str.length(), Size), res.begin());
        res[Size - 1] = '\0';
        return res;
    }

public:
    consteval CtString(const char (&str)[Size])
        : length(CtString<Size>::initLength(str))
        , array(CtString<Size>::initArray(str)) {}

    template <std::size_t S>
    consteval CtString(const char (&str)[S])
        : length(CtString<Size>::initLength(str))
        , array(CtString<Size>::initArray(str)) {}

    consteval CtString(const std::array<char, Size>& str)
        : length(initLength(str))
        , array(initArray(str)) {}

    template <std::size_t S>
    consteval CtString(const std::array<char, S>& str)
        : length(initLength(str))
        , array(initArray(str)) {}

    template <std::size_t S>
    consteval CtString(const CtString<S>& str)
        : length(str.length)
        , array(initArray(str.array)) {}

    consteval CtString(const std::string_view& str)
        : length(str.back() == '\0' ? str.length() : str.length() + 1)
        , array(initArray(str)) {}

    consteval std::string_view to_string_view() const {
        return std::string_view(array.data(), length - 1);
    }

    consteval auto begin() const { return array.begin(); }

    consteval auto end() const { return array.end(); }

    template <std::size_t OtherSize>
    consteval CtString<Size + OtherSize - 1>
    operator+(const CtString<OtherSize>& other) const {
        std::array<char, Size + OtherSize - 1> arr{};

        std::copy(this->begin(), this->end() - 1, arr.begin());
        std::copy(other.begin(), other.end() - 1, arr.begin() + Size - 1);
        arr.back() = '\0';

        return CtString<Size + OtherSize - 1>{arr};
    }

    template <std::size_t OtherSize>
    consteval CtString<Size + OtherSize - 1>
    operator+(const char (&str)[OtherSize]) const {
        return operator+(CtString{str});
    }

    template <std::size_t SepSize, std::size_t FirstSize, std::size_t... OtherSize>
        requires(sizeof...(OtherSize) > 0)
    static consteval auto AddSep(const CtString<SepSize>& sep,
                                 const CtString<FirstSize>& first,
                                 const CtString<OtherSize>&... other) {
        return first + ((sep + other) + ...);
    }

    template <std::size_t N>
        requires(N == 0)
    consteval CtString<1> repeat() const {
        return CtString<1>("");
    }

    template <std::size_t N>
        requires(N > 0)
    consteval CtString<N*(Size - 1) + 1> repeat() const {
        std::array<char, N*(Size - 1) + 1> arr{};

        for (std::size_t i = 0; i < N - 1; ++i) {
            auto offset = i * (Size - 1);
            std::copy(this->begin(), this->end() - 1, arr.begin() + offset);
        }
        arr.back() = '\0';

        return CtString<N*(Size - 1) + 1>{arr};
    }

    template <std::size_t N, std::size_t SepSize>
        requires(N == 0)
    consteval CtString<1> repeatSep(CtString<SepSize> sep) const {
        return CtString<1>("");
    }

    template <std::size_t N, std::size_t SepSize>
        requires(N > 0)
    consteval CtString<N*(Size - 1) + (N - 1) * (SepSize - 1) + 1>
    repeatSep(CtString<SepSize> sep) const {
        std::array<char, N*(Size - 1) + (N - 1) * (SepSize - 1) + 1> arr{};

        for (std::size_t i = 0; i < N - 1; ++i) {
            auto offset = i * ((Size - 1) + (SepSize - 1));
            std::copy(this->begin(), this->end() - 1, arr.begin() + offset);
            std::copy(sep.begin(), sep.end() - 1, arr.begin() + offset + Size - 1);
        }
        std::copy(this->begin(), this->end() - 1, arr.end() - Size);
        arr.back() = '\0';

        return CtString<N*(Size - 1) + (N - 1) * (SepSize - 1) + 1>{arr};
    }

    template <std::size_t N, std::size_t SepSize>
    consteval auto repeatSep(const char (&str)[SepSize]) const {
        return repeatSep<N>(CtString{str});
    }

    template <std::size_t S>
    consteval bool operator==(const CtString<S>& other) const {
        if (length != other.length) {
            return false;
        }
        for (std::size_t i = 0; i < length; ++i) {
            if (array[i] != other.array[i]) {
                return false;
            }
        }
        return true;
    }

    consteval bool operator==(const std::string_view& str) const {
        if (length != str.size() and length != (str.size() + 1)) {
            return false;
        }
        for (std::size_t i = 0; i < str.size(); ++i) {
            if (array[i] != str[i]) {
                return false;
            }
        }
        return true;
    }

    template <std::size_t S>
    consteval std::size_t count(const CtString<S>& pattern) const {
        auto this_str = to_string_view();
        auto pattern_str = pattern.to_string_view();

        std::size_t i = 0;
        std::size_t c = 0;
        while ((i = this_str.find(pattern_str, i)) != std::string::npos) {
            i += pattern_str.length();
            ++c;
        }
        return c;
    }

    const std::size_t length;
    const std::array<char, Size> array;
};

template <CtString R>
consteval auto operator"" _cts() {
    return R;
}

template <typename T>
struct IsCtStringHelper {
    template <std::size_t S>
    static constexpr std::true_type test(const CtString<S>*);
    static constexpr std::false_type test(...);
    using type = decltype(test(std::declval<T*>()));
};

template <typename T>
static constexpr bool IsCtStringV = IsCtStringHelper<T>::type::value;

template <typename T>
concept IsCtString = IsCtStringV<T>;

} // namespace gnev