#include "transform/3d/TransformFactory_3D.hpp"

namespace gnev {

TransformFactory_3D::TransformFactory_3D(Ref<TransformStorage_3D> storage)
    : storage(storage) {}

TransformFactory_3D::TransformFactory_3D(GLuint capacity)
    : storage(MakeSharable<TransformStorage_3D>(capacity)) {}

Ref<TransformStorage_3D> TransformFactory_3D::getStorage() const { return storage; }

Ref<Transform_3D> TransformFactory_3D::createTransform() const {
    return MakeSharable<Transform_3D>(storage);
}

} // namespace gnev