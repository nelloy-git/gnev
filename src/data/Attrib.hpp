#pragma once

#include "data/AttribInfo.hpp"

namespace gnev {

template<AttribInfo T>
struct Attrib final
{
    static constexpr AttribInfo info = T;
    using type = AttribType<T.type, info.elements>::type;
    using element_type = AttribType<T.type, info.elements>::element_type;

    Attrib(std::initializer_list<GLfloat> src);

    template<auto N = !T.normalized, std::enable_if_t<N, bool> = true>
    Attrib(std::initializer_list<GLint> src);

    template<auto N = !T.normalized, std::enable_if_t<N, bool> = true>
    Attrib(std::initializer_list<GLuint> src);
 
    void write(const GLfloat* src);

    template<auto N = !T.normalized, std::enable_if_t<N, bool> = true>
    void write(const GLint* src);

    template<auto N = !T.normalized, std::enable_if_t<N, bool> = true>
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
Attrib<T>::Attrib(std::initializer_list<GLfloat> src)
{
    this->write(src.begin());
}

template<AttribInfo T>
template<auto N, std::enable_if_t<N, bool>>
Attrib<T>::Attrib(std::initializer_list<GLint> src)
{
    this->write(src.begin());
}

template<AttribInfo T>
template<auto N, std::enable_if_t<N, bool>>
Attrib<T>::Attrib(std::initializer_list<GLuint> src)
{
    this->write(src.begin());
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
template<auto N, std::enable_if_t<N, bool>>
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
template<auto N, std::enable_if_t<N, bool>>
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