#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <limits>
#include <source_location>
#include <stdexcept>
#include <string_view>

namespace gnev {

template <std::size_t Size>
struct CtString;
constexpr std::size_t DEFAULT_CTSTRING_SIZE = 128;
using CtStrDef = CtString<DEFAULT_CTSTRING_SIZE>;

template <std::size_t Size>
struct CtString {
    consteval CtString(const char (&str)[Size])
        : length(initLength(str))
        , array(initArray(str)) {}

    consteval CtString(const std::array<char, Size>& str)
        : length(initLength(str))
        , array(initArray(str)) {}

    template <std::size_t S>
    consteval CtString(const char (&str)[S])
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
        : length(str.length())
        , array(initArray(str)) {}

    constexpr std::string_view to_string_view() const {
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
    static consteval std::array<char, Size> initArray(const char (&str)[S]) {
        std::array<char, Size> res = {};
        std::copy(std::begin(str), std::end(str), res.begin());
        return res;
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
};

constexpr CtString CtStringEmpty{""};

template <auto CtStr>
consteval auto CtStringOptimize() {
    constexpr std::size_t S = CtStr.length;
    std::array<char, S> arr = {};
    std::copy(CtStr.begin(), CtStr.begin() + S, arr.begin());
    return CtString{arr};
}

template <auto... CtStrs>
consteval std::size_t CtStringConcatLength() {
    return ((CtStrs.length - 1) + ... + 0) + 1;
}

template <auto... CtStrs, std::size_t Size = CtStringConcatLength<CtStrs...>()>
consteval CtString<Size> CtStringConcat() {
    std::array<char, Size> arr{};

    std::size_t p = 0;
    (
        [&arr, &p]() {
            std::copy(CtStrs.begin(), CtStrs.end() - 1, arr.begin() + p);
            p += CtStrs.length - 1;
        }(),
        ...);
    arr[Size - 1] = '\0';
    return CtString{arr};
}

template <auto CtStr, std::size_t N>
consteval auto CtStringRepeat() {
    if constexpr (N == 0) {
        return CtString<1>{""};
    } else {
        constexpr std::size_t L = N * (CtStr.length - 1) + 1;
        std::array<char, L> arr = {};
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < CtStr.length - 1; ++j) {
                arr[i * (CtStr.length - 1) + j] = CtStr.array.data()[j];
            }
        }
        arr[L - 1] = '\0';
        return CtString{arr};
    }
}

template <auto CtStr, auto CtStrSep, std::size_t N>
consteval auto CtStringRepeatSep() {
    if constexpr (N == 0) {
        return CtString<1>{""};
    } else {
        constexpr auto WithSep = CtStringConcat<CtStr, CtStrSep>();
        constexpr auto Repeated = CtStringRepeat<WithSep, N - 1>();
        return CtStringConcat<Repeated, CtStr>();
    }
}

consteval std::size_t cstrLength(const char* const str) {
    constexpr std::size_t MAX_LEN = std::string::npos;
    if (not str) {
        throw std::logic_error("cstrLength got nullptr");
    }

    size_t i = 0;
    while (i <= MAX_LEN and str[i] != '\0') {
        ++i;
    }

    return i;
}

template <std::size_t Size>
consteval auto toCtString(const char* const str) {
    std::array<char, Size> buffer = {};
    std::fill(buffer.begin(), buffer.end(), '\0');

    std::size_t str_length = cstrLength(str);
    if (str_length == std::string::npos) {
        throw std::out_of_range("str_length == std::string::npos");
    }
    if (Size < str_length) {
        throw std::out_of_range("Size < str_length");
    }
    std::copy(str, str + str_length, buffer.begin());
    buffer[Size - 1] = '\0';

    return CtString{buffer};
}

} // namespace gnev