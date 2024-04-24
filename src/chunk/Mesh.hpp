#pragma once

#include "Ctx.hpp"
#include "gl/Buffer.hpp"
#include "gl/VertexArray.hpp"
#include "gl/data/Index.hpp"
#include "gl/data/VertexInfo.hpp"
#include "util/Ref.hpp"

namespace gnev {

class Mesh {
public:
    using Loc = unsigned int;

    static constexpr GLuint BINDING_INDEX = 0;

    Mesh();
    virtual ~Mesh() = default;

    void setElements(const Ref<gl::Buffer>& indices, GLenum index_type);
    void setVertices(const Ref<gl::Buffer>& vertices,
                     const Ref<gl::VertexInfo> vertex_info);
    void bindAttribute(Loc shader_loc, Loc attrib_loc);
    void draw(std::size_t n_triangles, std::size_t first = 0) const;

private:
    Ref<gl::VertexArray> vao;

    Ptr<gl::Buffer> indices;
    GLenum index_type;

    Ptr<gl::Buffer> vertices;
    Ptr<gl::VertexInfo> vertex_info;
};

} // namespace gnev