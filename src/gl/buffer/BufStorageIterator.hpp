#pragma once

#include "gl/buffer/BufIterator.hpp"

namespace gnev::gl {

template <IsTriviallyCopyable T>
class BufStorage;

template <IsTriviallyCopyable T>
class EXPORT BufStorageIterator : public BufIterator<T> {
    friend BufStorage<T>;

public:
    ~BufStorageIterator();

    T getData() const override;
    void setData(const T&) override;

    T& operator*() override;
    const T& operator*() const override;

    T* operator->() override;
    const T* operator->() const override;

protected:
    BufStorageIterator(BufStorage<T>& buffer, GLuint index);
    BufStorageIterator(const BufStorage<T>& buffer, GLuint index);

private:
    mutable std::unique_ptr<T> data;
};

template <IsTriviallyCopyable T>
BufStorageIterator<T>::~BufStorageIterator() {
    if (data) {
        BufIterator<T>::getStorage().setSubData(BufIterator<T>::getIndex() * sizeof(T),
                                                sizeof(T),
                                                data.get());
    }
}

template <IsTriviallyCopyable T>
T BufStorageIterator<T>::getData() const {
    if (data) {
        BufIterator<T>::getStorage().getSubData(BufIterator<T>::getIndex() * sizeof(T),
                                                sizeof(T),
                                                data.get());
        return *data;
    } else {
        T value;
        BufIterator<T>::getStorage().getSubData(BufIterator<T>::getIndex() * sizeof(T),
                                                sizeof(T),
                                                &value);
        return value;
    }
}

template <IsTriviallyCopyable T>
void BufStorageIterator<T>::setData(const T& value) {
    if (data) {
        *data = value;
    }
    BufIterator<T>::getStorage().setSubData(BufIterator<T>::getIndex() * sizeof(T),
                                            sizeof(T),
                                            &value);
}

template <IsTriviallyCopyable T>
T& BufStorageIterator<T>::operator*() {
    if (not data) {
        data = std::make_unique_for_overwrite<T>();
        BufIterator<T>::getStorage().getSubData(BufIterator<T>::getIndex() * sizeof(T),
                                                sizeof(T),
                                                data.get());
    }
    return *data;
}

template <IsTriviallyCopyable T>
const T& BufStorageIterator<T>::operator*() const {
    if (not data) {
        data = std::make_unique_for_overwrite<T>();
        BufIterator<T>::getStorage().getSubData(BufIterator<T>::getIndex() * sizeof(T),
                                                sizeof(T),
                                                data.get());
    }
    return *data;
}

template <IsTriviallyCopyable T>
T* BufStorageIterator<T>::operator->() {
    if (not data) {
        data = std::make_unique_for_overwrite<T>();
        BufIterator<T>::getStorage().getSubData(BufIterator<T>::getIndex() * sizeof(T),
                                                sizeof(T),
                                                data.get());
    }
    return data.get();
}

template <IsTriviallyCopyable T>
const T* BufStorageIterator<T>::operator->() const {
    if (not data) {
        data = std::make_unique_for_overwrite<T>();
        BufIterator<T>::getStorage().getSubData(BufIterator<T>::getIndex() * sizeof(T),
                                                sizeof(T),
                                                data.get());
    }
    return data.get();
}

template <IsTriviallyCopyable T>
BufStorageIterator<T>::BufStorageIterator(BufStorage<T>& buffer, GLuint index)
    : BufIterator<T>(buffer, index) {}

template <IsTriviallyCopyable T>
BufStorageIterator<T>::BufStorageIterator(const BufStorage<T>& buffer, GLuint index)
    : BufIterator<T>(buffer, index) {}

} // namespace gnev::gl