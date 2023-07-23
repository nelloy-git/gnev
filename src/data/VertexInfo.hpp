#pragma once

#include "data/AttribInfo.hpp"
#include "data/AttribType.hpp"

#ifndef GNEV_VERTEX_INFO_MAX_ATTRIBUTES
#define GNEV_VERTEX_INFO_MAX_ATTRIBUTES 16
#endif

namespace gnev {

struct VertexInfo {
    template<size_t S>
    constexpr VertexInfo(std::array<AttribInfo, S> attribs);

    std::array<std::optional<AttribInfo>, GNEV_VERTEX_INFO_MAX_ATTRIBUTES> attribs;

    constexpr size_t get_size();

    template<std::size_t I>
    static constexpr GLuint get_offset();
    
private:
    static decltype(attribs) _init_attribs(std::initializer_list<AttribInfo> attribs);

    template<std::size_t... I>
    static constexpr GLuint _get_offset(const std::index_sequence<I...>&);

};

template<size_t S>
constexpr VertexInfo::VertexInfo(std::array<AttribInfo, S> init_attribs)
{
    size_t i = 0;
    for (auto& attrib : init_attribs){
        attribs[i] = attrib;
        ++i;
    }
}

constexpr size_t VertexInfo::get_size(){
    size_t size = 0;
    for (auto& attr : attribs){
        size += attr.has_value() ? attr.value().
    }
}

// template<AttribInfo ... Attribs>
// template<std::size_t I>
// constexpr GLuint VertexInfo<Attribs...>::get_size()
// {
//     return sizeof(std::tuple_element_t<I>);
// }

// template<AttribInfo ... Attribs>
// template<std::size_t I>
// constexpr GLuint VertexInfo<Attribs...>::get_offset()
// {
//     if constexpr (I == 0){
//         return 0;
//     } else {
//         return _get_offset(std::make_index_sequence<I>{});
//     }
// }

// template<AttribInfo ... Attribs>
// template<std::size_t... I>
// constexpr GLuint VertexInfo<Attribs...>::_get_offset(const std::index_sequence<I...>&)
// {
//     if constexpr (sizeof...(I) == 0){
//         return 0;
//     } else {
//         return (get_size<I>() + ...);
//     }
// }

}
