// #include "Camera.hpp"
// #include <stdexcept>

// Camera::Camera(const std::shared_ptr<Container>& container, const GL& initial)
//     : container{container}, range{container->pull(1, initial)} {}

// Camera::Accessor Camera::operator->(){
//     if (range.size == 0){
//         throw std::out_of_range("");
//     }
//     return container->at(range.begin);
// }

// const Camera::Accessor Camera::operator->() const {
//     if (range.size == 0){
//         throw std::out_of_range("");
//     }
//     return container->at(range.begin);
// }