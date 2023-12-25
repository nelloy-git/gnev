#include "mesh/3d/QuadMesh_3D.hpp"

#include "mesh/base/Index.hpp"

namespace gnev {

using IndexView = QuadMesh_3D::IndexView;
using DataView = QuadMesh_3D::DataView;

Ref<QuadMesh_3D> QuadMesh_3D::MakeDynamic(GLuint capacity){
    auto data_view = DataView::MakeDynamic(capacity);
    auto index_view = IndexView::MakeDynamic(capacity);
    return MakeSharable<QuadMesh_3D>(index_view, data_view);
}

QuadMesh_3D::QuadMesh_3D(const Ref<IndexView>& index_view, const Ref<DataView>& data_view)
    : vao(MakeSharable<gl::VertexArray>())
    , index_view(index_view)
    , data_view(data_view) {
    vao->setElementBuffer(index_view->accessor->buffer);
    vao->setVertexBuffer(BUFFER_BINDING,
                         data_view->accessor->buffer,
                         0,
                         sizeof(VertGLdata_3D));
}

Ref<IndexView> QuadMesh_3D::getIndexView() const { return index_view; }

Ref<DataView> QuadMesh_3D::getDataView() const { return data_view; }

Ref<Quad_3D> QuadMesh_3D::createQuad() const {
    return MakeSharable<Quad_3D>(index_view, data_view);
}

void QuadMesh_3D::bindAttribute(GLuint shader_loc, GLuint attrib_loc) {
    static constexpr base::VertexInfo VERT_INFO = VertGLdata_3D::info;

    if (shader_loc >= GL_MAX_VERTEX_ATTRIBS || attrib_loc >= VERT_INFO.count) {
        throw std::out_of_range("");
    }

    const base::VertexAttributeInfo& attr_info = VERT_INFO.attributes[attrib_loc];
    vao->glVertexArrayAttribBinding(shader_loc, BUFFER_BINDING);
    vao->glVertexArrayAttribFormat(shader_loc,
                                   attr_info.elements,
                                   attr_info.type,
                                   attr_info.normalized,
                                   VERT_INFO.offsets[attrib_loc]);
    vao->glEnableVertexArrayAttrib(shader_loc);
}

void QuadMesh_3D::draw() const {
    vao->glBindVertexArray();
    gl::Ctx::Get().glDrawElements(GL_TRIANGLES,
                                  (sizeof(IndexView::Data) / sizeof(VertGLindex_3D)) * index_view->getMaxUsed(),
                                  base::IndexEnum<VertGLindex_3D>,
                                  0);
}

} // namespace gnev