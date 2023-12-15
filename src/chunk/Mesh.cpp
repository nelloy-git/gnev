#include "chunk/Mesh.hpp"

#include <cstddef>
#include <stdexcept>

#include "gl/VertexArray.hpp"
#include "gl/data/Index.hpp"
#include "gl/data/VertexAttributeInfo.hpp"

namespace gnev {

Mesh::Mesh()
    : vao(MakeSharable<gl::VertexArray>())
    , index_type(0) {}

void Mesh::setElements(const Ref<gl::Buffer>& indices, GLenum index_type) {
    this->indices = indices;
    this->index_type = index_type;
}

void Mesh::setVertices(const Ref<gl::Buffer>& vertices,
                       const Ref<gl::VertexInfo> vertex_info) {
    this->vertices = vertices;
    this->vertex_info = vertex_info;
}

void Mesh::bindAttribute(Loc shader_loc, Loc attrib_loc) {
    if (not(indices and index_type and vertices and vertex_info)) {
        throw std::logic_error("");
    }

    if (shader_loc >= GL_MAX_VERTEX_ATTRIBS || attrib_loc >= vertex_info->count) {
        throw std::out_of_range("");
    }

    const gl::VertexAttributeInfo& attr_info = vertex_info->attributes[attrib_loc];
    vao->glVertexArrayAttribBinding(shader_loc, BINDING_INDEX);
    vao->glVertexArrayAttribFormat(shader_loc,
                                  attr_info.elements,
                                  attr_info.type,
                                  attr_info.normalized,
                                  vertex_info->offsets[attrib_loc]);
    vao->glEnableVertexArrayAttrib(shader_loc);
}

void Mesh::draw(std::size_t n_triangles, std::size_t first) const {
    auto offset = gl::GetIndexSize(index_type) * first;

    gl::Ctx::Get().glDrawElements(GL_TRIANGLES,
                                  3 * n_triangles,
                                  index_type,
                                  reinterpret_cast<const void*>(offset));
}
} // namespace gnev