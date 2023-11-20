#pragma once

#include <optional>

#include "gl/buffer/BufStorage.hpp"

namespace gnev::gl {

template <IsTriviallyCopyable T>
class BufStorageIndexMap : public BufStorage<T> {
public:
    using CleanUp = std::optional<std::function<void(BufStorageIndexMap&, GLuint)>>;

    BufStorageIndexMap(GLbitfield storage_flags, GLuint capacity, CleanUp clean_up = {});
    virtual ~BufStorageIndexMap();

    BufStorageIterator<T> operator[](GLuint index);
    const BufStorageIterator<T> operator[](GLuint index) const;

    BufStorageIterator<T> at(GLuint index);
    const BufStorageIterator<T> at(GLuint index) const;

    std::optional<GLuint> initUnusedIndex();
    void freeIndex(GLuint index);
    bool containsIndex(const GLuint& index) const;

private:
    CleanUp clean_up;
    std::unordered_set<GLuint> unused;
};

template <IsTriviallyCopyable T>
BufStorageIndexMap<T>::BufStorageIndexMap(GLbitfield storage_flags,
                                          GLuint capacity,
                                          CleanUp clean_up)
    : BufStorage<T>(storage_flags, capacity, {}, T{})
    , clean_up(clean_up) {
    for (GLuint i = 0; i < capacity; ++i) {
        unused.insert(unused.end(), capacity - i - 1);
    }
}

template <IsTriviallyCopyable T>
BufStorageIndexMap<T>::~BufStorageIndexMap() {}

template <IsTriviallyCopyable T>
BufStorageIterator<T> BufStorageIndexMap<T>::operator[](GLuint index) {
    return at(index);
}

template <IsTriviallyCopyable T>
const BufStorageIterator<T> BufStorageIndexMap<T>::operator[](GLuint index) const {
    return at(index);
}

template <IsTriviallyCopyable T>
BufStorageIterator<T> BufStorageIndexMap<T>::at(GLuint index) {
    if (unused.contains(index)) {
        throw std::out_of_range("");
    }
    return BufStorage<T>::operator[](index);
}

template <IsTriviallyCopyable T>
const BufStorageIterator<T> BufStorageIndexMap<T>::at(GLuint index) const {
    if (unused.contains(index)) {
        throw std::out_of_range("");
    }
    return BufStorage<T>::operator[](index);
}

template <IsTriviallyCopyable T>
std::optional<GLuint> BufStorageIndexMap<T>::initUnusedIndex() {
    auto iter = unused.begin();
    if (iter == unused.end()) {
        return std::nullopt;
    }
    GLuint index = *iter;
    unused.erase(iter);
    return index;
}

template <IsTriviallyCopyable T>
void BufStorageIndexMap<T>::freeIndex(GLuint index) {
    unused.insert(unused.end(), index);
    if (clean_up.has_value()) {
        clean_up.value()(*this, index);
    }
}

template <IsTriviallyCopyable T>
bool BufStorageIndexMap<T>::containsIndex(const GLuint& index) const {
    return not unused.contains(index);
};

} // namespace gnev::gl