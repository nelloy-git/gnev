#pragma once

#include "boost/preprocessor/punctuation/remove_parens.hpp"
#include "boost/preprocessor/seq/elem.hpp"
#include "boost/preprocessor/seq/for_each.hpp"
#include "boost/preprocessor/seq/for_each_i.hpp"
#include "boost/preprocessor/seq/to_tuple.hpp"

namespace gnev::gl {

namespace details {

#define GNEV_VERTEX_INFO(seq)                                                            \
    static constexpr std::array<gnev::gl::VertexAttributeInfo, BOOST_PP_SEQ_SIZE(seq)>   \
        Info = {BOOST_PP_REMOVE_PARENS(BOOST_PP_SEQ_TO_TUPLE(                            \
            BOOST_PP_SEQ_FOR_EACH(GNEV_VERTEX_INFO_LIST, _, seq)))}

#define GNEV_VERTEX_INFO_LIST(r, data, elem) (BOOST_PP_SEQ_ELEM(0, elem))

#define GNEV_VERTEX_ATTR(r, data, i, elem)                                               \
    gnev::gl::DeduceVertexAttributeType<BOOST_PP_SEQ_ELEM(0, elem)>::Buffer              \
        BOOST_PP_SEQ_ELEM(1, elem);

} // namespace details

#define GNEV_VERTEX(name, ...)                                                           \
    struct name {                                                                        \
        GNEV_VERTEX_INFO(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__));                         \
        BOOST_PP_SEQ_FOR_EACH_I(GNEV_VERTEX_ATTR,                                        \
                                _,                                                       \
                                BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))                   \
    }

// GNEV_TEMPLATE_CONCEPT(Vertex, ...)

} // namespace gnev::gl