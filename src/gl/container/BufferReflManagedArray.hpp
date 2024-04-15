// #pragma once

// #include <memory>
// #include <stdexcept>

// #include "gl/container/BufferReflAccessor.hpp"
// #include "gl/container/BufferReflArray.hpp"
// #include "util/Export.hpp"
// #include "util/IndexManager.hpp"

// namespace gnev::gl {

// template <IsReflStruct T>
// class EXPORT BufferReflManagedArray {
// public:
//     BufferReflManagedArray(std::unique_ptr<Buffer>&& buffer,
//                            std::unique_ptr<IBufferRawAccessor>&& accessor)
//         : manager{std::make_shared<IndexManager>(buffer->getSize() / sizeof(T))}
//         , array(std::make_unique<gl::BufferReflArray<T>>(std::move(buffer),
//                                                          std::move(accessor))) {}

//     virtual ~BufferReflManagedArray() = default;

//     std::optional<unsigned> reserveIndex() { return manager->reserveIndex(); }

//     bool freeIndex(unsigned index) { return manager->freeIndex(index); }

//     bool isInUse(unsigned index) const { return manager->isInUse(index); }

//     std::shared_ptr<unsigned> makeIndexGuard() {
//         return IndexManager::makeIndexGuard(manager);
//     }

//     gl::BufferReflAccessor<T> operator[](unsigned i) { return array->operator[](i); }

//     const gl::BufferReflAccessor<T> operator[](unsigned i) const {
//         return array->operator[](i);
//     }

//     gl::BufferReflAccessor<T> at(unsigned i) {
//         if (not isInUse(i)) {
//             throw std::out_of_range("");
//         }
//         return array->operator[](i);
//     }

//     const gl::BufferReflAccessor<T> at(unsigned i) const {
//         if (not isInUse(i)) {
//             throw std::out_of_range("");
//         }
//         return array->operator[](i);
//     }

// private:
//     std::shared_ptr<IndexManager> manager;
//     std::unique_ptr<gl::BufferReflArray<T>> array;
// };

// } // namespace gnev::gl