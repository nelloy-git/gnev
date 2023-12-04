#pragma once

#include "gl/BufferWrapper.hpp"
#include "transform/TransformGL.hpp"
#include "util/IndexStorage.hpp"

namespace gnev::base {

template <IsTransformGL T>
class TransformStorage;

template <IsTransformGL T>
using TransformStorageSetter =
    std::function<bool(TransformStorage<T>&, const void*, unsigned int, GLuint, GLuint)>;

template <IsTransformGL T>
using TransformStorageGetter =
    std::function<bool(const TransformStorage<T>&, void*, unsigned int, GLuint, GLuint)>;

template <IsTransformGL T>
class EXPORT TransformStorage
    : public BufferWrapper<TransformStorage<T>,
                           TransformStorageSetter<T>,
                           TransformStorageGetter<T>>
    , public IndexStorage {
public:
    TransformStorage(GLuint capacity);
    virtual ~TransformStorage();

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

} // namespace gnev::base
