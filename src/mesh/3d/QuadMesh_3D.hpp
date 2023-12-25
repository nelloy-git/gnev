#pragma once

#include "gl/VertexArray.hpp"
#include "mesh/3d/Quad_3D.hpp"

namespace gnev {

class EXPORT QuadMesh_3D {
public:
    using IndexView = Quad_3D::IndexView;
    using DataView = Quad_3D::DataView;

    static constexpr GLuint BUFFER_BINDING = 0;

    static Ref<QuadMesh_3D> MakeDynamic(GLuint capacity);

    QuadMesh_3D(const Ref<IndexView>& index_view, const Ref<DataView>& data_view);
    virtual ~QuadMesh_3D() = default;

    Ref<IndexView> getIndexView() const;
    Ref<DataView> getDataView() const;
    Ref<Quad_3D> createQuad() const;

    void bindAttribute(GLuint shader_loc, GLuint attrib_loc);
    void draw() const;

private:
    Ref<gl::VertexArray> vao;
    Ref<IndexView> index_view;
    Ref<DataView> data_view;
};
} // namespace gnev