#pragma once

#include "transform/3d/TransformGL_3D.hpp"
#include "transform/3d/Transform_3D.hpp"
#include "util/Ref.hpp"

namespace gnev {

class EXPORT TransformFactory_3D {
public:
    using DataView = Transform_3D::DataView;
    using DataElem = Transform_3D::DataElem;

    TransformFactory_3D(const Ref<DataView>& data_view);
    TransformFactory_3D(GLuint capacity);
    virtual ~TransformFactory_3D() = default;

    Ref<DataView> getDataView() const;
    Ref<Transform_3D> createTransform() const;

private:
    Ref<DataView> data_view;

    static Ref<DataView> initDefaultData(GLuint capacity);
};

} // namespace gnev
