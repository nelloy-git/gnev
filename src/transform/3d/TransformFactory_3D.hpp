#pragma once

#include "transform/3d/TransformGL_3D.hpp"
#include "transform/3d/Transform_3D.hpp"
#include "transform/base/TransformStorage.hpp"
#include "util/Ref.hpp"

namespace gnev {

using TransformGL_3D = TransformGL_3D;
using Transform_3D = Transform_3D;
using TransformStorage_3D = base::TransformStorage<TransformGL_3D>;

class EXPORT TransformFactory_3D {
public:
    TransformFactory_3D(Ref<TransformStorage_3D> storage);
    TransformFactory_3D(GLuint capacity);
    virtual ~TransformFactory_3D() = default;

    Ref<TransformStorage_3D> getStorage() const;
    Ref<Transform_3D> createTransform() const;

private:
    Ref<TransformStorage_3D> storage;
};

} // namespace gnev
