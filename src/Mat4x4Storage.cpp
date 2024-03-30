#include "Mat4x4Storage.hpp"

#include <memory>
#include <stdexcept>

#include "util/IndexManager.hpp"

namespace gnev {

Mat4x4Storage::Mat4x4Storage(std::unique_ptr<gl::BufferReflArray<Mat4x4_Refl>>&& array_)
    : manager{std::make_shared<IndexManager>(array_->size())}
    , array(std::move(array_)) {}

std::optional<unsigned> Mat4x4Storage::reserveIndex() { return manager->reserveIndex(); }

bool Mat4x4Storage::freeIndex(unsigned index) { return manager->freeIndex(index); }

bool Mat4x4Storage::isInUse(unsigned index) const {
    return manager->isInUse(index);
}

std::shared_ptr<unsigned> Mat4x4Storage::makeIndexGuard() {
    return IndexManager::makeIndexGuard(manager);
}

gl::BufferReflAccessor<Mat4x4_Refl> Mat4x4Storage::operator[](unsigned i){
    return array->operator[](i);
}

const gl::BufferReflAccessor<Mat4x4_Refl> Mat4x4Storage::operator[](unsigned i) const {
    return array->operator[](i);
}

gl::BufferReflAccessor<Mat4x4_Refl> Mat4x4Storage::at(unsigned i){
    if (not isInUse(i)){
        throw std::out_of_range("");
    }
    return array->operator[](i);
}

const gl::BufferReflAccessor<Mat4x4_Refl> Mat4x4Storage::at(unsigned i) const {
    if (not isInUse(i)){
        throw std::out_of_range("");
    }
    return array->operator[](i);
}



} // namespace gnev