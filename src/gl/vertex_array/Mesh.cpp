#include "gl/vertex_array/Mesh.hpp"

#include <cstddef>
#include <stdexcept>

#include "gl/VertexArray.hpp"
#include "gl/data/Index.hpp"
#include "gl/data/VertexAttributeInfo.hpp"

using namespace gnev;
using namespace gnev::gl;
using namespace gnev::gl::vertex_array;

Mesh::Mesh(const data::VertexInfo& vertex_info)
    : gl::VertexArray()
    , vertex_info(vertex_info)
    , index_enum(0) {}

Mesh::~Mesh() {}

void Mesh::bindElements(const gl::Buffer& index_buffer, GLenum index_type) {
    glVertexArrayElementBuffer(index_buffer.handle());
    index_enum = index_type;
}

void Mesh::bindVertices(const gl::Buffer& buffer) {
    glVertexArrayVertexBuffer(0, buffer.handle(), 0, vertex_info.size);
}

void Mesh::bindAttribute(Loc shader_loc, Loc attrib_loc) {
    if (shader_loc >= GL_MAX_VERTEX_ATTRIBS || attrib_loc >= vertex_info.count) {
        throw std::out_of_range("");
    }

    const data::VertexAttributeInfo& attr_info = vertex_info.attributes[attrib_loc];
    glVertexArrayAttribBinding(shader_loc, 0);
    glVertexArrayAttribFormat(shader_loc,
                              attr_info.elements,
                              attr_info.type,
                              attr_info.normalized,
                              vertex_info.offsets[attrib_loc]);
    glEnableVertexArrayAttrib(shader_loc);
}

void Mesh::draw(std::size_t n_triangles, std::size_t first) const {
    auto offset = data::GetIndexSize(index_enum) * first;
    Ctx::Get().glDrawElements(GL_TRIANGLES,
                              3 * n_triangles,
                              index_enum,
                              reinterpret_cast<const void*>(offset));
}