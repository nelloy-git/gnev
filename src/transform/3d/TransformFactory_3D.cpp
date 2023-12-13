#include "transform/3d/TransformFactory_3D.hpp"

namespace gnev {

using DataView = TransformFactory_3D::DataView;
using DataElem = TransformFactory_3D::DataElem;

TransformFactory_3D::TransformFactory_3D(const Ref<DataView>& data_view)
    : data_view(data_view) {}

TransformFactory_3D::TransformFactory_3D(GLuint capacity)
    : data_view(initDefaultData(capacity)) {}

Ref<DataView> TransformFactory_3D::getDataView() const { return data_view; }

Ref<Transform_3D> TransformFactory_3D::createTransform() const {
    return MakeSharable<Transform_3D>(data_view);
}

Ref<DataView> TransformFactory_3D::initDefaultData(GLuint capacity) {
    auto buffer = MakeSharable<gl::Buffer>();
    buffer->initStorage(capacity * sizeof(TransformGL_3D),
                        nullptr,
                        GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT |
                            GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    auto accessor = MakeSharable<gl::buffer::AccessorCoherent>(buffer);
    return MakeSharable<DataView>(accessor);
}

} // namespace gnev