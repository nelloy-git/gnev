#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <limits>
#include <source_location>
#include <stdexcept>
#include <string_view>

namespace gnev {

constexpr std::size_t DEFAULT_CTSTRING_SIZE = 128;

template <std::size_t Size = DEFAULT_CTSTRING_SIZE>
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
        requires(S <= Size)
    consteval CtString(const CtString<S>& str)
        : length(str.length)
        , array(initArray(str.array)) {}

    constexpr std::string_view to_string_view() const {
        return std::string_view(array.data(), length - 1);
    }

    consteval auto begin() const { return array.begin(); }

    consteval auto end() const { return array.end(); }

    const std::size_t length;
    const std::array<char, Size> array;

private:
    template <std::size_t S>
    static consteval std::size_t initLength(const char (&str)[S]) {
        auto term = std::find(std::begin(str), std::end(str), '\0');
        if (term == std::end(str)) {
            throw std::logic_error("CtString supports nullterm strings only");
        }
        return std::distance(std::begin(str), term + 1);
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
        return std::distance(std::begin(str), term + 1);
    }

    template <std::size_t S>
    static consteval std::array<char, Size> initArray(const std::array<char, S>& str) {
        std::array<char, Size> res = {};
        std::copy(std::begin(str), std::end(str), res.begin());
        return res;
    }
};

template <auto CtStr>
consteval auto CtStringOptimize() {
    constexpr std::size_t S = CtStr.length;
    std::array<char, S> arr = {};
    std::copy(CtStr.begin(), CtStr.begin() + S, arr.begin());
    return CtString{arr};
}

template <auto... CtStrs>
consteval std::size_t CtStringConcatLength() {
    return (CtStrs.length + ... + 0) - sizeof...(CtStrs) + 1;
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
    constexpr std::size_t MAX_LEN = std::numeric_limits<std::size_t>::max();
    if (not str) {
        throw std::logic_error("cstrLength got nullptr");
    }

    size_t i = 0;
    while (i <= MAX_LEN and str[i] != '\0') {
        ++i;
    }

    return i;
}

template <std::size_t Length = DEFAULT_CTSTRING_SIZE>
consteval auto toCtString(const char* const str) {
    std::array<char, Length> buffer = {};
    std::fill(buffer.begin(), buffer.end(), '\0');

    std::size_t str_length = cstrLength(str);
    if (str_length != 0) {
        std::copy(str, str + std::min(Length - 1, str_length - 1), buffer.begin());
        buffer[Length - 1] = '\0';
    }

    return CtString{buffer};
}

template <std::size_t Length = DEFAULT_CTSTRING_SIZE>
consteval auto
getFuncName(const std::source_location& src_loc = std::source_location::current()) {
    return toCtString<Length>(src_loc.function_name());
}

template <std::size_t Length = 128>
consteval auto
getMethodName(const std::source_location& src_loc = std::source_location::current()) {
    auto str = getFuncName<Length>(src_loc);
    auto func_name = str.to_string_view();

    std::size_t args_start = func_name.find('(');
    std::string_view no_args = func_name.substr(0, args_start);

    std::size_t method_start = no_args.find_last_of(':');
    std::string_view method_name = no_args.substr(method_start + 1);

    std::array<char, Length> arr = {};
    std::fill(arr.begin(), arr.end(), '\0');
    std::copy(method_name.data(),
              method_name.data() + std::min(method_name.length(), Length),
              arr.begin());
    arr[Length - 1] = '\0';

    return CtString{arr};
}

template <std::size_t Length = DEFAULT_CTSTRING_SIZE>
consteval auto
getClassName(const std::source_location& src_loc = std::source_location::current()) {
    constexpr auto namespaces_to_remove = std::array{
        "gnev::",
        "gnev::gl::",
    };

    auto str = getFuncName(src_loc);
    auto func_name = str.to_string_view();

    std::size_t class_start = func_name.find(' ');
    std::string_view no_res = func_name.substr(class_start + 1);
    std::size_t args_start = no_res.find('(');
    std::string_view no_args = no_res.substr(0, args_start);
    std::size_t method_start = no_args.find_last_of(':');
    std::string_view full_class = no_args.substr(0, method_start - 1);

    std::string_view class_name = full_class;
    for (auto& ns : namespaces_to_remove) {
        std::string str{ns};
        std::size_t pos = full_class.find(str);
        if (pos != std::string::npos and
            class_name.length() > full_class.length() - str.length()) {
            class_name = full_class.substr(pos + str.length());
        }
    }

    std::array<char, Length> arr = {};
    std::fill(arr.begin(), arr.end(), '\0');
    std::copy(class_name.data(),
              class_name.data() + std::min(class_name.length(), Length),
              arr.begin());
    arr[Length - 1] = '\0';

    return CtString{arr};
}

}