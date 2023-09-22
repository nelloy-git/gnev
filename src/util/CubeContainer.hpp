#pragma once

#include <initializer_list>
#include <optional>
#include <stdexcept>
#include <utility>

namespace gnev {

enum class CubeSide : unsigned char {
    Top,
    Bottom,
    Front,
    Back,
    Left,
    Right
};

template <typename T>
struct CubeContainer {
public:
    template <typename... TopA,
              typename... BottomA,
              typename... FrontA,
              typename... BackA,
              typename... LeftA,
              typename... RightA>
    CubeContainer(const std::tuple<TopA...>& top,
                  const std::tuple<BottomA...>& bottom,
                  const std::tuple<FrontA...>& front,
                  const std::tuple<BackA...>& back,
                  const std::tuple<LeftA...>& left,
                  const std::tuple<RightA...>& right);

    T& operator[](CubeSide side);
    const T& operator[](CubeSide side) const;

private:
    T _top;
    T _bottom;
    T _front;
    T _back;
    T _left;
    T _right;
};

template <typename T>
template <typename... TopA,
          typename... BottomA,
          typename... FrontA,
          typename... BackA,
          typename... LeftA,
          typename... RightA>
CubeContainer<T>::CubeContainer(const std::tuple<TopA...>& top,
                                const std::tuple<BottomA...>& bottom,
                                const std::tuple<FrontA...>& front,
                                const std::tuple<BackA...>& back,
                                const std::tuple<LeftA...>& left,
                                const std::tuple<RightA...>& right)
    : _top(std::make_from_tuple<T>(top))
    , _bottom(std::make_from_tuple<T>(bottom))
    , _front(std::make_from_tuple<T>(front))
    , _back(std::make_from_tuple<T>(back))
    , _left(std::make_from_tuple<T>(left))
    , _right(std::make_from_tuple<T>(right)) {}

template <typename T>
T& CubeContainer<T>::operator[](CubeSide side) {
    switch (side) {
        using enum CubeSide;
    case Top:
        return _top;
    case Bottom:
        return _bottom;
    case Front:
        return _front;
    case Back:
        return _back;
    case Left:
        return _left;
    case Right:
        return _right;

    default:
        throw std::out_of_range("");
    }
}

template <typename T>
const T& CubeContainer<T>::operator[](CubeSide side) const {
    switch (side) {
        using enum CubeSide;
    case Top:
        return _top;
    case Bottom:
        return _bottom;
    case Front:
        return _front;
    case Back:
        return _back;
    case Left:
        return _left;
    case Right:
        return _right;

    default:
        throw std::out_of_range("");
    }
}

} // namespace gnev