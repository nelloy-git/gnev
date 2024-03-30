#pragma once

#include <cstddef>
#include <type_traits>

#include "boost/preprocessor.hpp"
#include "util/CtString.hpp"

namespace gnev {

template <typename T>
concept IsReflStruct = requires() {
    typename T::Meta;
    T::Meta::Size;
    T::Meta::FieldsN;
    typename T::Meta::template FieldI<0>;
};

// Usage:
// Generate struct:
//   GNEV_REFL_STRUCT(<struct_name>, (alignas(<alignment>))(<type>)(<field_name>),
//   ...)
// Get field meta:
//   <struct_name>::Meta::Field<N>
//   <struct_name>::Meta::Field<CtString{"field_name"}>
//   <struct_name>::Meta::Field<&<struct_name>::<field_name>>
#define GNEV_REFL_STRUCT(name, ...)                                                      \
    GNEV_REFL_STRUCT_DATA_DECLARE(name);                                                 \
    GNEV_REFL_STRUCT_META_DECLARE();                                                     \
    GNEV_REFL_STRUCT_DATA_DEFINE(name, __VA_ARGS__);                                     \
    GNEV_REFL_STRUCT_META_DEFINE(name, __VA_ARGS__);

#define GNEV_REFL_STRUCT_DATA_DECLARE(name) struct name

#define GNEV_REFL_STRUCT_DATA_DEFINE(name, ...)                                          \
    struct name {                                                                        \
        using Meta = ReflStructMeta<name>;                                               \
        BOOST_PP_SEQ_FOR_EACH_I(GNEV_REFL_DEFINE_STRUCT_FIELD,                           \
                                ,                                                        \
                                BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))                   \
    }

#define GNEV_REFL_STRUCT_META_DECLARE()                                                  \
    template <typename T>                                                                \
    struct ReflStructMeta

#define GNEV_REFL_DEFINE_STRUCT_FIELD(_, __, i, field)                                   \
    BOOST_PP_SEQ_FOR_EACH(GNEV_REFL_CONCAT_STRUCT_FIELD, , field);

#define GNEV_REFL_CONCAT_STRUCT_FIELD(_, __, field) field

#define GNEV_REFL_STRUCT_META_DEFINE(name, ...)                                          \
    template <>                                                                          \
    struct ReflStructMeta<name> {                                                        \
        GNEV_REFL_STRUCT_META_ROOT(name, __VA_ARGS__);                                   \
        BOOST_PP_SEQ_FOR_EACH_I(GNEV_REFL_STRUCT_META_FIELD_DEFINE,                      \
                                name,                                                    \
                                BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))                   \
    }

#define GNEV_REFL_STRUCT_META_ROOT(name, ...)                                            \
    using Owner = name;                                                                  \
    static constexpr std::size_t Size = sizeof(struct_name);                             \
    static constexpr std::size_t FieldsN = BOOST_PP_VARIADIC_SIZE(__VA_ARGS__);          \
                                                                                         \
    template <std::size_t I>                                                             \
    struct FieldI;                                                                       \
    template <auto>                                                                      \
    struct FieldHelper;                                                                  \
    template <auto Key>                                                                  \
    using Field = FieldHelper<Key>::value

#define GNEV_REFL_STRUCT_META_FIELD_DEFINE(_, owner, i, field)                           \
    template <>                                                                          \
    struct FieldI<i> {                                                                   \
        GNEV_REFL_STRUCT_META_FIELD_INFO(owner, i, field);                               \
    };                                                                                   \
    template <>                                                                          \
    struct FieldHelper<i> {                                                              \
        using value = FieldI<i>;                                                         \
    };                                                                                   \
    template <>                                                                          \
    struct FieldHelper<gnev::CtString{                                                   \
        BOOST_PP_STRINGIZE(BOOST_PP_SEQ_ELEM(2, field))}> {                              \
        using value = FieldI<i>;                                                         \
    };                                                                                   \
    template <>                                                                          \
    struct FieldHelper<&owner::BOOST_PP_SEQ_ELEM(2, field)> {                            \
        using value = FieldI<i>;                                                         \
    };

#define GNEV_REFL_STRUCT_META_FIELD_INFO(owner, i, field)                                \
    using Type = BOOST_PP_SEQ_ELEM(1, field);                                            \
                                                                                         \
    static constexpr std::size_t I = i;                                                  \
    static constexpr gnev::CtString Name =                                               \
        BOOST_PP_STRINGIZE(BOOST_PP_SEQ_ELEM(2, field));                                 \
    static constexpr auto Ptr = &owner::BOOST_PP_SEQ_ELEM(2, field);                     \
                                                                                         \
    static constexpr std::size_t Size = sizeof(Type);                                    \
    static constexpr std::size_t Offset = offsetof(owner, BOOST_PP_SEQ_ELEM(2, field))

namespace details {

template <IsReflStruct Parent, auto FieldKey>
auto GetReflMeta() {
    using ChildMeta = typename Parent::Meta::template Field<FieldKey>;
    return ChildMeta{};
}

template <IsReflStruct Parent, auto FirstFieldKey, auto... FieldKeys>
    requires(sizeof...(FieldKeys) > 0)
auto GetReflMeta() {
    using ChildMeta = typename Parent::Meta::template Field<FirstFieldKey>;
    return GetReflMeta<typename ChildMeta::Type, FieldKeys...>();
}

template <IsReflStruct Parent, auto FieldKey>
consteval std::size_t GetReflOffset(std::size_t offset) {
    using ChildMeta = typename Parent::Meta::template Field<FieldKey>;
    return offset + ChildMeta::Offset;
}

template <IsReflStruct Parent, auto FirstFieldKey, auto... FieldKeys>
    requires(sizeof...(FieldKeys) > 0)
consteval std::size_t GetReflOffset(std::size_t offset) {
    using ChildMeta = typename Parent::Meta::template Field<FirstFieldKey>;
    return GetReflOffset<typename ChildMeta::Type, FieldKeys...>(offset +
                                                                 ChildMeta::Offset);
}

} // namespace details

template <IsReflStruct Parent, auto FirstFieldKey, auto... FieldKeys>
using ReflMeta = decltype(details::GetReflMeta<Parent, FirstFieldKey, FieldKeys...>());

template <IsReflStruct Parent, auto FirstFieldKey, auto... FieldKeys>
consteval std::size_t GetReflOffset() {
    return details::GetReflOffset<Parent, FirstFieldKey, FieldKeys...>(0);
}

} // namespace gnev

GNEV_REFL_STRUCT(struct_name, (alignas(16))(int)(aaa1), (alignas(16))(int)(aaa2))
