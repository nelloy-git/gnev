#pragma once

#include "gl/buffer/iterator/ConstBufIterator.hpp"

namespace gnev::gl {

template <IsTriviallyCopyable T>
class BufStorage;

template <IsTriviallyCopyable T>
class EXPORT ConstBufStorageIterator : public ConstBufIterator<T> {
public:
    ConstBufStorageIterator(BufStorage<T>& storage, GLuint index);
    ~ConstBufStorageIterator();

    BufStorage<T>& getStorage() const;

    void getData(T& dst) const override;
    void copyTo(void* dst, std::size_t gl_offset, std::size_t n_bytes) const override;

    void setData(const T&) override;
    void copyFrom(const void* src, std::size_t gl_offset, std::size_t n_bytes) const override;
};

template <IsTriviallyCopyable T>
ConstBufStorageIterator<T>::ConstBufStorageIterator(BufStorage<T>& buffer, GLuint index)
    : ConstBufIterator<T>(buffer, index) {}

template <IsTriviallyCopyable T>
ConstBufStorageIterator<T>::~ConstBufStorageIterator() {}

template <IsTriviallyCopyable T>
BufStorage<T>& ConstBufStorageIterator<T>::getStorage() const {
    return dynamic_cast<BufStorage<T>&>(ConstBufIterator<T>::getBuffer());
}

template <IsTriviallyCopyable T>
void ConstBufStorageIterator<T>::getData(T& dst) const {
    ConstBufIterator<T>::getStorage().getSubData(ConstBufIterator<T>::getIndex() * sizeof(T),
                                            sizeof(T),
                                            &dst);
}

template <IsTriviallyCopyable T>
void ConstBufStorageIterator<T>::copyTo(void* dst, std::size_t gl_offset, std::size_t n_bytes) const {
    if (gl_offset + n_bytes >= sizeof(T)) {
        throw std::out_of_range("");
    }

    auto buf_offset = ConstBufIterator<T>::getIndex() * sizeof(T) + gl_offset;
    ConstBufIterator<T>::getStorage().getSubData(ConstBufIterator<T>::getIndex() * sizeof(T),
                                            sizeof(T),
                                            dst);
}

} // namespace gnev::gl