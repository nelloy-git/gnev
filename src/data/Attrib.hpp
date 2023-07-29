#pragma once

#include "data/AttribInfo.hpp"

namespace gnev {

template<AttribInfo T>
struct Attrib final
{
    static constexpr AttribInfo info = T;
    using type = AttribType<T.type, info.elements>::type;
    using element_type = AttribType<T.type, info.elements>::element_type;

    Attrib(const type& src);

    template<auto E = info.elements, auto P = info.is_packed, std::enable_if_t<E==1 && !P, bool> = true>
    Attrib(const element_type& v0);

    template<auto E = info.elements, auto P = info.is_packed, std::enable_if_t<E==2 && !P, bool> = true>
    Attrib(const element_type& v0, const element_type& v1);

    template<auto E = info.elements, auto P = info.is_packed, std::enable_if_t<E==3 && !P, bool> = true>
    Attrib(const element_type& v0, const element_type& v1, const element_type& v2);

    template<auto E = info.elements, auto P = info.is_packed, std::enable_if_t<E==4 && !P, bool> = true>
    Attrib(const element_type& v0, const element_type& v1, const element_type& v2, const element_type& v3);
 
    void write(const GLfloat* src);

    template<auto N = T.normalized, std::enable_if_t<!N, bool> = true>
    void write(const GLint* src);

    template<auto N = T.normalized, std::enable_if_t<!N, bool> = true>
    void write(const GLuint* src);

    type data;
};
        
template<typename derived>
struct is_VertAttrib
{
    template<AttribInfo T>
    static constexpr std::true_type  test(const Attrib<T> *);
    static constexpr std::false_type test(...);
    static constexpr bool value = decltype(test(std::declval<derived*>()))::value;
};

template<typename T>
concept IsVertAttrib = is_VertAttrib<T>::value;


template<AttribInfo T>
Attrib<T>::Attrib(const type& src)
    : data(src)
{
}

template<AttribInfo T>
template<auto E, auto P, std::enable_if_t<E==1 && !P, bool>>
Attrib<T>::Attrib(const element_type& v0)
    : data({v0})
{
}

template<AttribInfo T>
template<auto E, auto P, std::enable_if_t<E==2 && !P, bool>>
Attrib<T>::Attrib(const element_type& v0, const element_type& v1)
    : data({v0, v1})
{
}

template<AttribInfo T>
template<auto E, auto P, std::enable_if_t<E==3 && !P, bool>>
Attrib<T>::Attrib(const element_type& v0, const element_type& v1, const element_type& v2)
    : data({v0, v1, v2})
{
}

template<AttribInfo T>
template<auto E, auto P, std::enable_if_t<E==4 && !P, bool>>
Attrib<T>::Attrib(const element_type& v0, const element_type& v1, const element_type& v2, const element_type& v3)
    : data({v0, v1, v2, v3})
{
}

template<AttribInfo T>
void Attrib<T>::write(const GLfloat* src)
{
    if constexpr (info.is_packed){
        []<bool B = false>(){static_assert(B, "Unsupported ElementT");}();
    } else {
        for (int i = 0; i < info.elements; ++i){
            if constexpr (!info.normalized){
                data[i] = src[i];
            } else {
                GLdouble normed;
                if constexpr (info.is_signed){
                    normed = std::max(-1.0f, std::min(1.0f, src[i]));
                } else {
                    normed = std::max( 0.0f, std::min(1.0f, src[i]));
                }

                if constexpr (info.type == GL_FLOAT){
                    data[i] = normed;
                } else {
                    data[i] = normed >= 0 ? normed * std::numeric_limits<element_type>::max() : normed * std::numeric_limits<element_type>::min();
                }
            }
        }
    }
}

template<AttribInfo T>
template<auto N, std::enable_if_t<!N, bool>>
void Attrib<T>::write(const GLint* src)
{
    if constexpr (info.is_packed){
        []<bool B = false>(){static_assert(B, "Unsupported ElementT");}();
    } else {
        for (int i = 0; i < info.elements; ++i){
            if constexpr (!info.normalized){
                data[i] = src[i];
            }
        }
    }
}

template<AttribInfo T>
template<auto N, std::enable_if_t<!N, bool>>
void Attrib<T>::write(const GLuint* src)
{
    if constexpr (info.is_packed){
        []<bool B = false>(){static_assert(B, "Unsupported ElementT");}();
    } else {
        for (int i = 0; i < info.elements; ++i){
            if constexpr (!info.normalized){
                data[i] = src[i];
            }
        }
    }
}


}