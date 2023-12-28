#pragma once

#include "gl/buffer/IndexMapViewElem.hpp"
#include "glm/glm.hpp"

namespace gnev {

class EXPORT Mat4x4Storage {
public:
    using MatGLdata = glm::mat4x4;
    using DataView = gl::buffer::IndexMapView<glm::mat4>;
    using DataElem = gl::buffer::IndexMapViewElem<glm::mat4>;

    static Ref<Mat4x4Storage> MakeCoherent(GLuint capacity);

    Mat4x4Storage(const Ref<DataView> data_view);
    virtual ~Mat4x4Storage() = default;

    Ref<gl::Buffer> getBuffer() const;

    Ref<DataElem> lockMat(const MatGLdata& initial);

private:
    Ref<DataView> data_view;
};
} // namespace gnev