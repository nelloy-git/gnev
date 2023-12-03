#pragma once

#include <iostream>

#include "gl/Buffer.hpp"
#include "material/base/MaterialGL.hpp"
#include "util/Export.hpp"
#include "util/Ref.hpp"

namespace gnev::base {

template <IsMaterialGL T>
class EXPORT MaterialDataStorage {
public:
    // clang-format off
    using Setter =
        std::function<bool(gl::Buffer& buffer, GLuint offset, GLuint size, const void* src)>;
    using Getter =
        std::function<bool(const gl::Buffer& buffer, GLuint offset, GLuint size, void* dst)>;
    // clang-format on

    MaterialDataStorage(Ref<gl::Buffer> buffer,
                        std::initializer_list<GLuint> contains_indices = {});
    MaterialDataStorage(GLuint capacity);
    virtual ~MaterialDataStorage();

    Ref<gl::Buffer> getBuffer() const;
    void setSetter(const Setter& setter);
    void setGetter(const Getter& getter);

    GLuint initIndex();
    void freeIndex(GLuint index);
    bool hasIndex(GLuint index) const;
    GLuint unusedCount() const;

    template <typename V>
    bool setData(GLuint index, const V* src, GLuint offset, GLuint size = sizeof(V));
    template <typename V>
    bool getData(GLuint index, V* dst, GLuint offset, GLuint size = sizeof(V)) const;

    // Default setter
    static bool
    setSubData(gl::Buffer& buffer, GLuint offset, GLuint size, const void* src);
    // Default getter
    static bool
    getSubData(const gl::Buffer& buffer, GLuint offset, GLuint size, void* dst);

private:
    Ref<gl::Buffer> buffer;
    Setter buffer_setter;
    Getter buffer_getter;
    GLuint capacity;
    std::unordered_set<GLuint> unused;

    static Ref<gl::Buffer> initBuffer(GLuint capacity);
    static GLuint calcCapacity(gl::Buffer& buffer);
};

template <IsMaterialGL T>
MaterialDataStorage<T>::MaterialDataStorage(Ref<gl::Buffer> buffer,
                                            std::initializer_list<GLuint>
                                                contains_indices)
    : buffer(buffer)
    , capacity(calcCapacity(buffer))
    , buffer_setter(&setSubData)
    , buffer_getter(&getSubData) {
    for (GLuint i = 0; i < capacity; ++i) {
        unused.emplace(capacity - i - 1);
    }
    for (auto index : contains_indices) {
        unused.extract(index);
    }
}

template <IsMaterialGL T>
MaterialDataStorage<T>::MaterialDataStorage(GLuint capacity)
    : MaterialDataStorage(initBuffer(capacity)) {}

template <IsMaterialGL T>
MaterialDataStorage<T>::~MaterialDataStorage() {}

template <IsMaterialGL T>
Ref<gl::Buffer> MaterialDataStorage<T>::getBuffer() const {
    return buffer;
}

template <IsMaterialGL T>
void MaterialDataStorage<T>::setSetter(const Setter& setter) {
    buffer_setter = setter;
}

template <IsMaterialGL T>
void MaterialDataStorage<T>::setGetter(const Getter& getter) {
    buffer_getter = getter;
}

template <IsMaterialGL T>
GLuint MaterialDataStorage<T>::initIndex() {
    auto iter = unused.begin();
    if (iter == unused.end()) {
        throw std::out_of_range("");
    }
    GLuint index = *iter;
    unused.erase(iter);
    return index;
}

template <IsMaterialGL T>
void MaterialDataStorage<T>::freeIndex(GLuint index) {
    unused.insert(unused.end(), index);
}

template <IsMaterialGL T>
bool MaterialDataStorage<T>::hasIndex(GLuint index) const {
    return not unused.contains(index);
}

template <IsMaterialGL T>
GLuint MaterialDataStorage<T>::unusedCount() const {
    return unused.size();
}

template <IsMaterialGL T>
template <typename V>
bool MaterialDataStorage<T>::setData(GLuint index,
                                     const V* src,
                                     GLuint offset,
                                     GLuint size) {
    if (offset + size > sizeof(T)) {
        throw std::out_of_range("");
    }
    return buffer_setter(buffer, index * sizeof(T) + offset, size, src);
}

template <IsMaterialGL T>
template <typename V>
bool MaterialDataStorage<T>::getData(GLuint index,
                                     V* src,
                                     GLuint offset,
                                     GLuint size) const {
    if (offset + size > sizeof(T)) {
        throw std::out_of_range("");
    }
    std::cout << "read index: " << index << std::endl;
    return buffer_getter(buffer, index * sizeof(T) + offset, size, src);
}

template <IsMaterialGL T>
bool MaterialDataStorage<T>::setSubData(gl::Buffer& buffer,
                                        GLuint offset,
                                        GLuint size,
                                        const void* src) {
    std::cout << "write: " << offset << " (" << size << ")" << std::endl;
    buffer.setSubData(offset, size, src);
    return true;
};

template <IsMaterialGL T>
bool MaterialDataStorage<T>::getSubData(const gl::Buffer& buffer,
                                        GLuint offset,
                                        GLuint size,
                                        void* dst) {
    std::cout << "read: " << offset << " (" << size << ")" << std::endl;
    buffer.getSubData(offset, size, dst);
    return true;
};

template <IsMaterialGL T>
Ref<gl::Buffer> MaterialDataStorage<T>::initBuffer(GLuint capacity) {
    auto buffer = MakeSharable<gl::Buffer>();
    buffer->initStorage(capacity * sizeof(T),
                        nullptr,
                        GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT);
    return buffer;
}

template <IsMaterialGL T>
GLuint MaterialDataStorage<T>::calcCapacity(gl::Buffer& buffer) {
    GLuint buf_size;
    buffer.getParameteriv(GL_BUFFER_SIZE, (GLint*)&buf_size);
    return buf_size / sizeof(T);
}

} // namespace gnev::base