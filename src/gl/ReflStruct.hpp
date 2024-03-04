#pragma once

#include <cstddef>
#include <string_view>
#include <tuple>

#include "boost/preprocessor.hpp"
#include "util/CtString.hpp"

namespace gnev {

template <typename T>
concept IsReflStruct = requires() {
    T::Meta;
    T::Meta::Size;
    T::Meta::FieldsN;
};

// Usage:
// Generate struct:
//   GNEV_REFL_STRUCT_DECLARE(<struct_name>, ((alignas(<align>)) <type>) <field_name>,
//   ...)
// Get field meta:
//   <struct_name>::Meta::FieldByName<"field_name">
//   <struct_name>::Meta::FieldByI<N>
#define GNEV_REFL_STRUCT_DECLARE(struct_name, ...)                                       \
    template <typename T>                                                                \
    struct ReflStructMetaData;                                                           \
    struct struct_name {                                                                 \
        using Meta = ReflStructMetaData<struct_name>;                                    \
        BOOST_PP_SEQ_FOR_EACH(GNEV_REFL_STRUCT_DECLARE_FIELD_EACH,                       \
                              data,                                                      \
                              BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))                     \
    };                                                                                   \
                                                                                         \
    GNEV_REFL_STRUCT_DECLARE_META_DATA(struct_name, __VA_ARGS__);

#define GNEV_REFL_STRUCT_DECLARE_FIELD_EACH(r, data, x) GNEV_TRIO(x);
#define GNEV_REFL_STRUCT_DECLARE_META_DATA(struct_name, ...)                             \
    template <>                                                                          \
    struct ReflStructMetaData<struct_name> {                                             \
        static constexpr std::size_t Size = sizeof(struct_name);                         \
        static constexpr std::size_t FieldsN = BOOST_PP_VARIADIC_SIZE(__VA_ARGS__);      \
                                                                                         \
        GNEV_REFL_STRUCT_DECLARE_META_DATA_FIELDS(struct_name, __VA_ARGS__)              \
    }

#define GNEV_REFL_STRUCT_DECLARE_META_DATA_FIELDS(struct_name, ...)                      \
    template <std::size_t I>                                                             \
    struct FieldByI;                                                                     \
    template <gnev::CtString Name>                                                       \
    struct FieldByName;                                                                  \
    BOOST_PP_SEQ_FOR_EACH_I(GNEV_REFL_STRUCT_DECLARE_META_DATA_FIELD_EACH,               \
                            struct_name,                                                 \
                            BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define GNEV_REFL_STRUCT_DECLARE_META_DATA_FIELD_EACH(r, struct_name, i, trio)           \
    template <>                                                                          \
    struct FieldByI<i> {                                                                 \
        GNEV_REFL_STRUCT_DECLARE_META_DATA_FIELD_META(struct_name, i, trio)              \
    };                                                                                   \
    template <>                                                                          \
    struct FieldByName<BOOST_PP_STRINGIZE(GNEV_TRIO_THIRD(trio))> {                      \
        GNEV_REFL_STRUCT_DECLARE_META_DATA_FIELD_META(struct_name, i, trio)              \
    };

#define GNEV_REFL_STRUCT_DECLARE_META_DATA_FIELD_META(struct_name, i, trio)              \
    using Owner = struct_name;                                                           \
    using Type = GNEV_TRIO_SECOND(trio);                                                 \
                                                                                         \
    static constexpr std::size_t I = i;                                                  \
    static constexpr std::size_t Size = sizeof(Type);                                    \
    static constexpr std::size_t Offset = offsetof(struct_name, GNEV_TRIO_THIRD(trio));  \
    static constexpr std::string_view Name = BOOST_PP_STRINGIZE(GNEV_TRIO_THIRD(trio));

#define GNEV_REM(...) __VA_ARGS__
#define GNEV_EAT(...)

#define GNEV_PAIR_FIRST(pair) GNEV_DETAIL_PAIR_FIRST(GNEV_DETAIL_PAIR_FIRST_PROBE pair, )
#define GNEV_DETAIL_PAIR_FIRST(...) GNEV_DETAIL_PAIR_FIRST_HEAD(__VA_ARGS__)
#define GNEV_DETAIL_PAIR_FIRST_HEAD(pair, ...) GNEV_REM pair
#define GNEV_DETAIL_PAIR_FIRST_PROBE(...) (__VA_ARGS__),
#define GNEV_PAIR_SECOND(pair) GNEV_EAT pair

#define GNEV_TRIO_FIRST(trio) GNEV_PAIR_FIRST(GNEV_PAIR_FIRST(trio))
#define GNEV_TRIO_SECOND(trio) GNEV_PAIR_SECOND(GNEV_PAIR_FIRST(trio))
#define GNEV_TRIO_THIRD(trio) GNEV_PAIR_SECOND(trio)
#define GNEV_TRIO(trio) GNEV_TRIO_FIRST(trio) GNEV_TRIO_SECOND(trio) GNEV_TRIO_THIRD(trio)

GNEV_REFL_STRUCT_DECLARE(TestRefl,
                         ((alignas(16)) int)field_1,
                         ((alignas(4)) unsigned)field_2);
GNEV_REFL_STRUCT_DECLARE(TestRefl2,
                         ((alignas(8)) int)field_1,
                         ((alignas(4)) unsigned)field_2);
} // namespace gnev