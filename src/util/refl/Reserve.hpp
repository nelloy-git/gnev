#pragma once

#include <cstddef>

namespace gnev::refl {

template <std::size_t S>
struct Reserve;

template <>
struct Reserve<0> {};

template <std::size_t S>
struct Reserve {
    static_assert(sizeof(char) == 1);
private:
    char _[S];
};

}