#pragma once

#include "gl/Buffer.hpp"
#include "gl/Ctx.hpp"
#include "gl/VertexArray.hpp"
#include "gl/data/Index.hpp"
#include "gl/data/VertexInfo.hpp"

namespace gnev::gl::vertex_array {

class Mesh : public gl::VertexArray {
public:
    using Loc = unsigned int;

    Mesh(const data::VertexInfo& vertex_info);
    virtual ~Mesh();

    void bindElements(const gl::Buffer& index_buffer, GLenum index_type);
    void bindVertices(const gl::Buffer& buffer);
    void bindAttribute(Loc shader_loc, Loc attrib_loc);

    void draw(std::size_t n_triangles, std::size_t first = 0) const;

private:
    data::VertexInfo vertex_info;
    GLenum index_enum;
};

} // namespace gnev::gl::data