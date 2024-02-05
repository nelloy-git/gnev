#pragma once

#include <source_location>
#include <vector>

#include "util/CtString.hpp"

namespace gnev {

class SrcLoc {
public:
    static consteval SrcLoc
    Current(const std::source_location& src_loc = std::source_location::current()) {
        return SrcLoc{src_loc};
    }

    std::string_view std_file_name;
    std::string_view std_function_name;

    std::string_view full_func_name;
    std::string_view short_func_name;
    std::string_view class_name;
    std::string_view result_name;

    consteval bool funcHasReturn() const { return result_name != "void"; }

private:
    consteval SrcLoc(const std::source_location& src_loc)
        : src_loc{src_loc}
        , std_file_name{src_loc.file_name()}
        , std_function_name{src_loc.function_name()}
        , full_func_name{initFullFuncName()}
        , short_func_name{initShortFuncName()}
        , class_name{initClassName()}
        , result_name{initResultName()} {}
        
    const std::source_location src_loc;

    static consteval void fillWords(std::vector<std::string_view>& dst,
                                    const std::string_view& str,
                                    const std::string_view& separators) {
        std::size_t start = 0;
        while (start != std::string::npos) {
            std::size_t end = str.find_first_of(separators, start + 1);
            if (end == std::string::npos) {
                end = str.length();
            }
            dst.push_back(str.substr(start, end - start));
            start = str.find_first_not_of(separators, end + 1);
        }
    }

    static consteval void fillWordEnds(std::vector<std::size_t>& dst,
                                       const std::string_view& str,
                                       const std::string_view& separators) {
        std::size_t start = 0;
        while (start != std::string::npos) {
            std::size_t end = str.find_first_of(separators, start + 1);
            if (end == std::string::npos) {
                end = str.length();
            }
            dst.push_back(end);
            start = str.find_first_not_of(separators, end + 1);
        }
    }

    consteval std::size_t getArgsStart() const {
        return std_function_name.find_last_of('(');
    };

    consteval std::size_t getArgsLength() const {
        auto start = getArgsStart();
        auto end = std_function_name.find_last_of(')');
        if (start == std::string::npos or end == std::string::npos) {
            return 0;
        }
        return end - start;
    };

    consteval std::string_view initFullFuncName() const {
        auto args_start = getArgsStart();
        if (args_start == std::string::npos) {
            return std::string_view("");
        }

        auto no_args = std_function_name.substr(0, getArgsStart());
        std::vector<std::string_view> space_separated;
        fillWords(space_separated, no_args, {" "});

        // waiting for at least 2 words "RetType Full::Func::Name"
        if (space_separated.size() < 2) {
            return std::string_view("");
        }

        return space_separated.back();
    }

    consteval std::string_view initShortFuncName() const {
        std::vector<std::string_view> colon_separated;
        fillWords(colon_separated, full_func_name, {":"});

        if (colon_separated.size() == 0) {
            return std::string_view("");
        }

        return colon_separated.back();
    };

    consteval std::string_view initClassName() const {
        std::vector<std::string_view> colon_separated;
        fillWords(colon_separated, full_func_name, {":"});

        if (colon_separated.size() <= 1) {
            return std::string_view("");
        }

        return colon_separated[colon_separated.size() - 2];
    };

    consteval std::string_view initResultName() const {
        auto args_start = getArgsStart();
        if (args_start == std::string::npos) {
            return std::string_view("");
        }

        auto no_args = std_function_name.substr(0, getArgsStart());
        std::vector<std::string_view> space_separated;
        fillWords(space_separated, no_args, {" "});
#ifdef WIN32
        // ctor
        if (space_separated.size() <= 2) {
            return std::string_view("");
        }

        std::vector<std::size_t> word_ends;
        fillWordEnds(word_ends, no_args, {" "});
        return no_args.substr(0, word_ends[space_separated.size() - 3]);
#else
        static_assert(false, "TODO");
#endif
    };
};

} // namespace gnev