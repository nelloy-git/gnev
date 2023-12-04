#pragma once

#include <unordered_set>

#include "gl/Buffer.hpp"
#include "gl/BufferWrapper.hpp"
#include "material/base/MaterialGL.hpp"
#include "util/Export.hpp"
#include "util/IndexStorage.hpp"
#include "util/Ref.hpp"

namespace gnev::base {

template <IsMaterialGL T>
class MaterialDataStorage;

template <IsMaterialGL T>
using MaterialDataStorageSetter = std::function<
    bool(MaterialDataStorage<T>&, const void*, unsigned int, GLuint, GLuint)>;

template <IsMaterialGL T>
using MaterialDataStorageGetter = std::function<
    bool(const MaterialDataStorage<T>&, void*, unsigned int, GLuint, GLuint)>;

template <IsMaterialGL T>
class EXPORT MaterialDataStorage
    : public BufferWrapper<MaterialDataStorage<T>,
                           MaterialDataStorageSetter<T>,
                           MaterialDataStorageGetter<T>>
    , public IndexStorage {
public:
    // TODO
    // MaterialDataStorage(Ref<gl::Buffer> buffer,
    //                     Ref<Setter> setter,
    //                     Ref<Getter> getter,
    //                     std::initializer_list<GLuint> contains_indices = {});
    MaterialDataStorage(GLuint capacity);
    virtual ~MaterialDataStorage();

    // Default setter
    static bool setSubData(MaterialDataStorage& buffer,
                           const void* src,
                           unsigned int index,
                           GLuint offset,
                           GLuint size);
    // Default getter
    static bool getSubData(const MaterialDataStorage& buffer,
                           void* dst,
                           unsigned int index,
                           GLuint offset,
                           GLuint size);

private:
    static Ref<gl::Buffer> initBuffer(GLuint capacity);
};

template <IsMaterialGL T>
MaterialDataStorage<T>::MaterialDataStorage(GLuint capacity)
    : BufferWrapper<MaterialDataStorage<T>,
                    MaterialDataStorageSetter<T>,
                    MaterialDataStorageGetter<T>>(initBuffer(capacity),
                                                  &setSubData,
                                                  &getSubData)
    , IndexStorage(capacity) {}

template <IsMaterialGL T>
MaterialDataStorage<T>::~MaterialDataStorage() {}

template <IsMaterialGL T>
bool MaterialDataStorage<T>::setSubData(MaterialDataStorage& storage,
                                        const void* src,
                                        unsigned int index,
                                        GLuint offset,
                                        GLuint size) {
    storage.getBuffer()->setSubData(index * sizeof(T) + offset, size, src);
    return true;
};

template <IsMaterialGL T>
bool MaterialDataStorage<T>::getSubData(const MaterialDataStorage& storage,
                                        void* dst,
                                        unsigned int index,
                                        GLuint offset,
                                        GLuint size) {
    storage.getBuffer()->getSubData(index * sizeof(T) + offset, size, dst);
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

} // namespace gnev::base