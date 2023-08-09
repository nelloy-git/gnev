#pragma once

#include <memory>

namespace gnev {

template<typename T>
struct Cube {
    using size_type = unsigned int;

    T user_data;
    size_type x = 0;
    size_type y = 0;
    size_type z = 0;
    size_type size_x = 0;
    size_type size_y = 0;
    size_type size_z = 0;
};

}