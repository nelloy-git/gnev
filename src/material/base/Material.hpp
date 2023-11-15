#pragma once

#include <memory>
#include <stdexcept>

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

    virtual GLuint getDataIndex() const;
    virtual Data getData() const;
    virtual void setData(const Data&);

private:
    std::weak_ptr<MaterialStorage<T>> weak_storage;
    GLuint index;
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
    if (!store){
        throw std::runtime_error("");
    }
    return store;
}

template <IsTriviallyCopyable T>
GLuint Material<T>::getDataIndex() const {
    return index;
}

template <IsTriviallyCopyable T>
Material<T>::Data Material<T>::getData() const {
    auto store = getStorage();
    return getStorage()->getDataStorage()[index].getData();
}

template <IsTriviallyCopyable T>
void Material<T>::setData(const Data& data) {
    auto store = getStorage();
    return getStorage()->getDataStorage()[index].setData(data);
}

} // namespace gnev::base