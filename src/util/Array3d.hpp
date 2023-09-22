#pragma once

#include <vector>

namespace gnev {

template <typename T>
class Array3d {
public:
    using size_type = std::size_t;

    Array3d(size_type size_x, size_type size_y, size_type size_z, const T& initial_value = T{});
    virtual ~Array3d();

    size_type size_x() const;
    size_type size_y() const;
    size_type size_z() const;

    T& operator()(size_type x, size_type y, size_type z);
    const T& operator()(size_type x, size_type y, size_type z) const;

private:
    size_type _size_x;
    size_type _size_y;
    size_type _size_z;
    std::vector<std::vector<std::vector<T>>> _array;
};

template <typename T>
Array3d<T>::Array3d(size_type size_x, size_type size_y, size_type size_z, const T& initial_value)
    : _size_x(size_x)
    , _size_y(size_y)
    , _size_z(size_z)
    , _array(size_x, std::vector<std::vector<T>>(size_y, std::vector<T>(size_z, initial_value))) {}

template <typename T>
Array3d<T>::~Array3d() {}

template <typename T>
inline typename Array3d<T>::size_type Array3d<T>::size_x() const {
    return _size_x;
}

template <typename T>
inline typename Array3d<T>::size_type Array3d<T>::size_y() const {
    return _size_y;
}

template <typename T>
inline typename Array3d<T>::size_type Array3d<T>::size_z() const {
    return _size_z;
}

template <typename T>
T& Array3d<T>::operator()(size_type x, size_type y, size_type z) {
    return _array[x][y][z];
};

template <typename T>
const T& Array3d<T>::operator()(size_type x, size_type y, size_type z) const {
    return _array[x][y][z];
};

} // namespace gnev