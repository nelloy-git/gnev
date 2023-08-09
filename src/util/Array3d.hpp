#pragma once

#include <vector>

namespace gnev {

template<typename T>
class Array3d {
public:
    using size_type = unsigned int;

    Array3d(size_type size_x, size_type size_y, size_type size_z, const T& initial_value = T{});
    virtual ~Array3d();

    const size_type size_x;
    const size_type size_y;
    const size_type size_z;

    T& operator()(size_type x, size_type y, size_type z);
    const T& operator()(size_type x, size_type y, size_type z) const;

private:
    std::vector<std::vector<std::vector<T>>> _array;

};

template<typename T>
Array3d<T>::Array3d(size_type size_x, size_type size_y, size_type size_z, const T& initial_value)
    : size_x(size_x),
      size_y(size_y),
      size_z(size_z),
      _array(size_x, std::vector<std::vector<T>>(size_y, std::vector<T>(size_z, initial_value)))
{
};

template<typename T>
Array3d<T>::~Array3d()
{
};

template<typename T>
T& Array3d<T>::operator()(size_type x, size_type y, size_type z)
{
    return _array[x][y][z];
};

template<typename T>
const T& Array3d<T>::operator()(size_type x, size_type y, size_type z) const
{
    return _array[x][y][z];
};

}