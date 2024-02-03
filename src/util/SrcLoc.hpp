#pragma once

#include <source_location>

#include "util/CtString.hpp"

namespace gnev {

class SrcLoc {
public:
    static constexpr std::size_t STR_SIZE = 256;

    consteval SrcLoc(const std::source_location& src_loc =
                         std::source_location::current())
        : file_name{toCtString<STR_SIZE>(src_loc.file_name())}
        , function_name{toCtString<STR_SIZE>(src_loc.function_name())}
        , class_name{initClassName()}
        , method_name{initMethodName()}
        , result_name{initResultName()}
        , line{src_loc.line()}
        , column{src_loc.column()} {}

    SrcLoc(const SrcLoc&) = delete;
    SrcLoc(SrcLoc&&) = delete;

    CtString<STR_SIZE> file_name;
    CtString<STR_SIZE> function_name;
    CtString<STR_SIZE> class_name;
    CtString<STR_SIZE> method_name;
    CtString<STR_SIZE> result_name;
    unsigned line;
    unsigned column;

    constexpr std::string_view getClassName() const { return class_name.to_string_view(); }

    constexpr std::string_view getMethodName() const { return method_name.to_string_view(); }

    consteval bool doReturnVoid() const {
        return result_name == CtString{"void"};
    }

private:
    consteval CtString<STR_SIZE> initClassName() const {
        constexpr auto namespaces_to_remove = std::array{
            "gnev::",
            "gnev::gl::",
        };

        std::string_view func_name = function_name.to_string_view();
        std::string_view no_res = func_name.substr(func_name.find(' ') + 1);
        std::string_view no_args = no_res.substr(0, no_res.find('(') + 1);
        std::string_view full_class = no_args.substr(0, no_args.find_last_of(':'));

        std::string_view class_name = full_class;
        for (auto& ns : namespaces_to_remove) {
            std::string str{ns};
            std::size_t pos = full_class.find(str);
            if (pos != std::string::npos and
                class_name.length() > full_class.length() - str.length()) {
                class_name = full_class.substr(pos + str.length());
            }
        }

        return class_name;
    }

    consteval CtString<STR_SIZE> initMethodName() const {
        std::string_view func_name = function_name.to_string_view();
        std::string_view no_args = func_name.substr(0, func_name.find('(') + 1);
        std::string_view method_name = no_args.substr(no_args.find_last_of(':') + 1);
        return method_name;
    }

    consteval CtString<STR_SIZE> initResultName() const {
        std::string_view func_name = function_name.to_string_view();
        std::string_view result_name = func_name.substr(0, func_name.find(' ') + 1);
        return result_name;
    }
};

} // namespace gnev