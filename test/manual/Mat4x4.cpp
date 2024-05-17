#include "Mat4x4.hpp"

#include <stdexcept>

namespace {

unsigned initIndex(Mat4x4::Container& container, const Mat4x4::GL& initial) {
    auto range = container.pull(1);
    while (range.size != 1) {
        container.reserve(2 * container.capacity());
        range = container.pull(1);
    }
    container.fill(range, initial);
    return range.begin;
}

} // namespace

Mat4x4::Mat4x4(const std::shared_ptr<Container>& container, const GL& initial)
    : weak_container(container)
    , container_index(initIndex(*container, initial)) {}

Mat4x4::~Mat4x4() {
    auto container = weak_container.lock();
    if (container) {
        container->push(gnev::gl::Range{.begin = container_index, .size = 1});
    }
}

unsigned Mat4x4::index() const { return container_index; }

gnev::gl::BufferReflAccessor<Mat4x4::GL> Mat4x4::operator->() {
    auto container = weak_container.lock();
    if (not container) {
        throw std::runtime_error("");
    }
    return weak_container.lock()->at(container_index);
}

const gnev::gl::BufferReflAccessor<Mat4x4::GL> Mat4x4::operator->() const {
    auto container = weak_container.lock();
    if (not container) {
        throw std::runtime_error("");
    }
    return weak_container.lock()->at(container_index);
}