#pragma once

#include <cstddef>
#include <string_view>

#include "boost/preprocessor.hpp"
#include "util/CtString.hpp"

namespace gnev {

template <typename T>
concept IsReflStruct = requires() {
    typename T::Meta;
    T::Meta::Size;
    T::Meta::FieldsN;
    typename T::Meta::template FieldMeta<0>;
};

// Usage:
// Generate struct:
//   GNEV_REFL_DEFINE_STRUCT(<struct_name>, (alignas(<alignment>))(<type>)(<field_name>),
//   ...)
// Get field meta:
//   <struct_name>::Meta::Field<N>
//   <struct_name>::Meta::Field<CtString{"field_name"}>
//   <struct_name>::Meta::Field<&<struct_name>::<field_name>>
#define GNEV_REFL_STRUCT(name, ...)                                                      \
    GNEV_REFL_STRUCT_META_DECLARE;                                                       \
    GNEV_REFL_STRUCT_DATA_DEFINE(name, __VA_ARGS__);                                     \
    GNEV_REFL_STRUCT_META_DEFINE(name, __VA_ARGS__);

#define GNEV_REFL_STRUCT_META_DECLARE                                                    \
    template <typename T>                                                                \
    struct ReflStructMeta

#define GNEV_REFL_STRUCT_DATA_DEFINE(name, ...)                                          \
    struct name {                                                                        \
        using Meta = ReflStructMeta<name>;                                               \
        BOOST_PP_SEQ_FOR_EACH(GNEV_REFL_DEFINE_STRUCT_FIELD,                             \
                              _,                                                         \
                              BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))                     \
    }

#define GNEV_REFL_DEFINE_STRUCT_FIELD(_, __, x)                                          \
    BOOST_PP_SEQ_FOR_EACH(GNEV_REFL_CONCAT_STRUCT_FIELD, __, x);

#define GNEV_REFL_CONCAT_STRUCT_FIELD(_, __, x) x

#define GNEV_REFL_STRUCT_META_DEFINE(name, ...)                                          \
    template <>                                                                          \
    struct ReflStructMeta<name> {                                                        \
        using Owner = name;                                                              \
        static constexpr std::size_t FieldsN = BOOST_PP_VARIADIC_SIZE(__VA_ARGS__);      \
    }

} // namespace gnev

GNEV_REFL_STRUCT(struct_name, (alignas(16))(int)(aaa1), (alignas(16))(int)(aaa2))