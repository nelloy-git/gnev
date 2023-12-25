#pragma once

#include <limits>

#include "gl/buffer/IndexMapViewElem.hpp"
#include "mesh/3d/QuadGLdata_3D.hpp"

namespace gnev {

class EXPORT Quad_3D {
public:
    using IndexView = gl::buffer::IndexMapView<QuadGLindex_3D>;
    using IndexElem = gl::buffer::IndexMapViewElem<QuadGLindex_3D>;

    using DataView = gl::buffer::IndexMapView<QuadGLdata_3D>;
    using DataElem = gl::buffer::IndexMapViewElem<QuadGLdata_3D>;

    static constexpr GLuint InvalidIndex = std::numeric_limits<GLuint>::max();

    Quad_3D(const WeakRef<IndexView>& weak_index_view,
            const WeakRef<DataView>& weak_data_view);
    virtual ~Quad_3D() = default;

    void setQuad(const QuadGLdata_3D& quadrangle);
    void setVert(GLuint i, const VertGLdata_3D& vertex);

protected:
    Ref<IndexElem> index;
    Ref<DataElem> data;

    static void cleanupIndex(Ref<IndexView>& view, GLuint index);
    static void cleanupQuad(Ref<DataView>& view, GLuint index);

private:
};
} // namespace gnev