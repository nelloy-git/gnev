#pragma once

#include <memory>
#include <stdexcept>

#include "gl/buffer/BufStorageIterator.hpp"
#include "material/base/Defines.hpp"

namespace gnev::base {

template <IsTriviallyCopyable T>
class MaterialStorage;

template <IsTriviallyCopyable T>
class EXPORT Material {
public:
    using Data = T;

    Material(const std::shared_ptr<MaterialStorage<T>>& storage, GLuint index);
    virtual ~Material();

    std::shared_ptr<MaterialStorage<T>> getStorage() const;

    gl::BufStorageIterator<T> getDataIterator();
    const gl::BufStorageIterator<T> getDataIterator() const;

    GLuint getDataIndex() const;
    Data getData() const;
    void setData(const Data&);
    void changeData(const std::function<void(Data&)>& apply);

private:
    std::weak_ptr<MaterialStorage<T>> weak_storage;
    GLuint index;

    gl::BufStorageIterator<T> getIterator();
};

namespace details {

template <typename derived>
struct is_Material {
    template <IsTriviallyCopyable T>
    static constexpr std::true_type test(const Material<T>*);
    static constexpr std::false_type test(...);
    static constexpr bool value = decltype(test(std::declval<derived*>()))::value;
};

} // namespace details

template <typename T>
concept IsMaterial = details::is_Material<T>::value;

template <IsTriviallyCopyable T>
Material<T>::Material(const std::shared_ptr<MaterialStorage<T>>& storage, GLuint index)
    : weak_storage(storage)
    , index(index) {}

template <IsTriviallyCopyable T>
Material<T>::~Material() {}

template <IsTriviallyCopyable T>
std::shared_ptr<MaterialStorage<T>> Material<T>::getStorage() const {
    auto store = weak_storage.lock();
    if (!store) {
        throw std::runtime_error("");
    }
    return store;
}

template <IsTriviallyCopyable T>
gl::BufStorageIterator<T> Material<T>::getDataIterator() {
    return getStorage()->getDataStorage()[index];
}

template <IsTriviallyCopyable T>
const gl::BufStorageIterator<T> Material<T>::getDataIterator() const {
    return getStorage()->getDataStorage()[index];
}

template <IsTriviallyCopyable T>
GLuint Material<T>::getDataIndex() const {
    return index;
}

template <IsTriviallyCopyable T>
Material<T>::Data Material<T>::getData() const {
    return getStorage()->getDataStorage()[index].getData();
}

template <IsTriviallyCopyable T>
void Material<T>::setData(const Data& data) {
    return getStorage()->getDataStorage()[index].setData(data);
}

template <IsTriviallyCopyable T>
void Material<T>::changeData(const std::function<void(Data&)>& apply) {
    auto iter = getDataIterator();
    apply(*iter);
}

} // namespace gnev::base