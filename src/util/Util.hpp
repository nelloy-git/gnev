#pragma once

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/greater_equal.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/punctuation/remove_parens.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/to_tuple.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#define GNEV_TEMPLATE_CONCEPT(name, ...)                                                 \
    namespace details {                                                                  \
    template <typename Derived>                                                          \
    struct BOOST_PP_CAT(test, name) {                                                    \
        template <                                                                       \
            GNEV_TEMPLATE_CONCEPT_NAMED_ARGS(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))>     \
        static constexpr std::true_type                                                  \
        test(const name<                                                                 \
             GNEV_TEMPLATE_CONCEPT_ARG_NAMES(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))>*);  \
        static constexpr std::false_type test(...);                                      \
        static constexpr bool value = decltype(test(std::declval<Derived*>()))::value;   \
    };                                                                                   \
    }                                                                                    \
    template <typename T>                                                                \
    static constexpr bool BOOST_PP_CAT(BOOST_PP_CAT(is, name), V) =                      \
        details::BOOST_PP_CAT(test, name)<T>::value;                                     \
    template <typename T>                                                                \
    concept BOOST_PP_CAT(Is, name) = BOOST_PP_CAT(BOOST_PP_CAT(is, name), V)<T>;

#define GNEV_TEMPLATE_CONCEPT_NAMED_ARGS(seq)                                            \
    BOOST_PP_REMOVE_PARENS(BOOST_PP_SEQ_TO_TUPLE(                                        \
        BOOST_PP_SEQ_FOR_EACH_I(GNEV_TEMPLATE_CONCEPT_NAMED_ARGS_EACH, _, seq)))

#define GNEV_TEMPLATE_CONCEPT_NAMED_ARGS_EACH(r, data, i, elem) (elem T##i)

#define GNEV_TEMPLATE_CONCEPT_ARG_NAMES(seq)                                             \
    BOOST_PP_REMOVE_PARENS(BOOST_PP_SEQ_TO_TUPLE(                                        \
        BOOST_PP_SEQ_FOR_EACH_I(GNEV_TEMPLATE_CONCEPT_ARG_NAMES_EACH, _, seq)))

#define GNEV_TEMPLATE_CONCEPT_ARG_NAMES_EACH(r, size, i, elem) (T##i)