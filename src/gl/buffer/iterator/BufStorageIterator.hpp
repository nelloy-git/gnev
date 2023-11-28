#pragma once

#include "gl/buffer/iterator/BufIterator.hpp"

namespace gnev::gl {

template <IsTriviallyCopyable T>
class BufStorage;

template <IsTriviallyCopyable T>
class EXPORT BufStorageIterator : public BufIterator<T> {
public:
    BufStorageIterator(BufStorage<T>& storage, GLuint index);
    ~BufStorageIterator();

    BufStorage<T>& getStorage() const;

    void getData(T& dst) const override;
    void copyTo(void* dst, std::size_t gl_offset, std::size_t n_bytes) const override;

    void setData(const T&) override;
    void copyFrom(const void* src, std::size_t gl_offset, std::size_t n_bytes) const override;
};

template <IsTriviallyCopyable T>
BufStorageIterator<T>::BufStorageIterator(BufStorage<T>& buffer, GLuint index)
    : BufIterator<T>(buffer, index) {}

template <IsTriviallyCopyable T>
BufStorageIterator<T>::~BufStorageIterator() {}

template <IsTriviallyCopyable T>
BufStorage<T>& BufStorageIterator<T>::getStorage() const {
    return dynamic_cast<BufStorage<T>&>(BufIterator<T>::getBuffer());
}

template <IsTriviallyCopyable T>
void BufStorageIterator<T>::getData(T& dst) const {
    BufIterator<T>::getStorage().getSubData(BufIterator<T>::getIndex() * sizeof(T),
                                            sizeof(T),
                                            &dst);
}

template <IsTriviallyCopyable T>
void BufStorageIterator<T>::setData(const T& value) {
    BufIterator<T>::getStorage().setSubData(BufIterator<T>::getIndex() * sizeof(T),
                                            sizeof(T),
                                            &value);
}

template <IsTriviallyCopyable T>
void BufStorageIterator<T>::copyTo(void* dst, std::size_t gl_offset, std::size_t n_bytes) const {
    if (gl_offset + n_bytes >= sizeof(T)) {
        throw std::out_of_range("");
    }

    auto buf_offset = BufIterator<T>::getIndex() * sizeof(T) + gl_offset;
    BufIterator<T>::getStorage().getSubData(BufIterator<T>::getIndex() * sizeof(T),
                                            sizeof(T),
                                            dst);
}

template <IsTriviallyCopyable T>
void BufStorageIterator<T>::copyFrom(const void* src, std::size_t gl_offset, std::size_t n_bytes) const {
    if (gl_offset + n_bytes >= sizeof(T)) {
        throw std::out_of_range("");
    }

    auto buf_offset = BufIterator<T>::getIndex() * sizeof(T) + gl_offset;
    BufIterator<T>::getStorage().setSubData(BufIterator<T>::getIndex() * sizeof(T),
                                            sizeof(T),
                                            src);
}

} // namespace gnev::gl