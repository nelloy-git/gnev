#pragma once

#include <array>
#include <concepts>
#include <source_location>
#include <string_view>

consteval std::size_t cstrLength(const char* const str, std::size_t max_length) {
    size_t i = 0;
    if (str)
        for (; i < max_length and str[i] != '\0'; ++i)
            ;
    return i;
}

template <std::size_t Length>
struct CtString {
    consteval CtString(const char (&str)[Length]) {
        length = cstrLength(str, Length);
        std::copy(std::begin(str), std::end(str), data.begin());
    }

    consteval CtString(const std::array<char, Length>& str) {
        length = cstrLength(str.data(), Length);
        std::copy(std::begin(str), std::end(str), data.begin());
    }

    static consteval auto toCtString(const char* const str) {
        std::size_t len = cstrLength(str, 512);

        std::array<char, Length> arr = {};
        std::copy(str, str + std::min(len, Length), arr.begin());
        return CtString<Length>(arr);
    }

    std::size_t length;
    std::array<char, Length> data = {};

    constexpr std::string_view to_string_view() const {
        return std::string_view(data.data(), length);
    }

    consteval bool isNullTerm() const { return length < Length; }

    consteval auto begin() const { return data.begin(); }

    consteval auto end() const { return isNullTerm() ? data.end() - 1 : data.end(); }

    template <std::size_t N>
    consteval auto repeat() const {
        if constexpr (N == 0) {
            return CtString<1>{""};
        } else {
            std::array<char, N* length> arr = {};
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < length; ++j) {
                    arr[i * length + j] = data.data()[j];
                }
            }
            return CtString{arr};
        }
    }
};

template <auto... CtStrs>
consteval auto CtStringConcat() {
    std::array<char, (CtStrs.length + ... + 0)> arr{};

    std::size_t p = 0;
    (
        [&arr, &p]() {
            std::copy(CtStrs.begin(), CtStrs.end(), arr.begin() + p);
            p += CtStrs.length;
        }(),
        ...);
    return CtString{arr};
}

template <auto CtStr, std::size_t N>
consteval auto CtStringRepeat() {
    if constexpr (N == 0) {
        return CtString<1>{""};
    } else {
        std::array<char, N* CtStr.length> arr = {};
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < CtStr.length; ++j) {
                arr[i * CtStr.length + j] = CtStr.data.data()[j];
            }
        }
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

template <std::size_t MaxLength = 128>
consteval auto
getFuncName(const std::source_location& src_loc = std::source_location::current()) {
    auto str = src_loc.function_name();

    std::array<char, MaxLength> arr = {};
    std::copy(str, str + cstrLength(str, MaxLength), arr.begin());
    return CtString{arr};
}

template <std::size_t MaxLength = 128>
consteval auto
getClassName(const std::source_location& src_loc = std::source_location::current()) {
    auto str = getFuncName(src_loc);
    auto func_name = str.to_string_view();

    std::size_t class_start = func_name.find(' ');
    std::string_view no_res = func_name.substr(class_start + 1);

    std::size_t args_start = no_res.find('(');
    std::string_view no_args = no_res.substr(0, args_start);

    std::size_t method_start = no_args.find_last_of(':');
    std::string_view full_class = no_args.substr(0, method_start - 1);

    std::size_t namespace_start = full_class.find("gnev::gl::");
    std::string_view class_name =
        full_class.substr(std::min(sizeof("gnev::gl::") - 1, full_class.length()));

    std::array<char, MaxLength> arr = {};
    std::copy(class_name.data(),
              class_name.data() + std::min(class_name.length(), MaxLength),
              arr.begin());
    return CtString{arr};
}

template <std::size_t MaxLength = 128>
consteval auto
getMethodName(const std::source_location& src_loc = std::source_location::current()) {
    auto str = getFuncName(src_loc);
    auto func_name = str.to_string_view();

    std::size_t args_start = func_name.find('(');
    std::string_view no_args = func_name.substr(0, args_start);

    std::size_t method_start = no_args.find_last_of(':');
    std::string_view method_name = no_args.substr(method_start + 1);

    std::array<char, MaxLength> arr = {};
    std::copy(method_name.data(),
              method_name.data() + std::min(method_name.length(), MaxLength),
              arr.begin());

    return CtString{arr};
}