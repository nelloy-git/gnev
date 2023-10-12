#pragma once

#include "gl/Buffer.hpp"
#include "gl/Ctx.hpp"
#include "gl/VertexArray.hpp"
#include "gl/data/Index.hpp"
#include "gl/data/VertexInfo.hpp"

namespace gnev::gl::data {

using Mesh = gl::VertexArray;

class MeshedBuffer : public Mesh {
public:
    using Loc = unsigned int;

    MeshedBuffer(const VertexInfo& vertex_info);
    virtual ~MeshedBuffer();

    void bindElements(const gl::Buffer& index_buffer, GLenum index_type);
    void bindVertices(const gl::Buffer& buffer);
    void bindAttribute(Loc shader_loc, Loc mesh_loc);

    void draw(std::size_t n_triangles, std::size_t first = 0) const;

private:
    VertexInfo vertex_info;
    GLenum index_enum;
};

} // namespace gnev::gl::data