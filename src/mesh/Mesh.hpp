#pragma once

#include "gl/VertexArray.hpp"
#include "gl/buffer/view/BufferViewMap.hpp"
#include "mesh/base/Index.hpp"
#include "mesh/base/Vertex.hpp"

namespace gnev {

template <base::IsVertex Vertex>
class EXPORT Mesh {
public:
    static constexpr GLuint BUFFER_BINDING = 0;

    static Ref<Mesh> MakeDynamicStorage(GLuint capacity) {
        auto index_view = gl::BufferViewMap<unsigned>::MakeDynamicStorage(capacity);
        auto vertex_view = gl::BufferViewMap<Vertex>::MakeDynamicStorage(capacity);
        return MakeSharable<Mesh>(index_view, vertex_view);
    }

    Mesh(Ref<gl::BufferViewMap<unsigned>> index_view,
         Ref<gl::BufferViewMap<Vertex>> vertex_view)
        : vao{MakeSharable<gl::VertexArray>()}
        , index_view{index_view}
        , vertex_view{vertex_view} {}

    void bindAttribute(GLuint shader_loc, GLuint attrib_loc) {
        static constexpr base::VertexInfo VERT_INFO = Vertex::info;

        if (shader_loc >= GL_MAX_VERTEX_ATTRIBS || attrib_loc >= VERT_INFO.count) {
            throw std::out_of_range("");
        }

        const base::VertexAttributeInfo& attr_info = VERT_INFO.attributes[attrib_loc];
        vao->setAttributeBinding(shader_loc, BUFFER_BINDING);
        vao->setAttributeFormat(shader_loc,
                                attr_info.elements,
                                attr_info.type,
                                attr_info.normalized,
                                VERT_INFO.offsets[attrib_loc]);
        vao->enableAttribute(shader_loc);
    }

    void draw() const {
        vao->bind();
        gl::Ctx::Get().glDrawElements(GL_TRIANGLES,
                                      index_view->getCount(),
                                      base::IndexEnum<VertGLindex_3D>,
                                      0);
    }

private:
    Ref<gl::VertexArray> vao;
    Ref<gl::BufferViewMap<unsigned>> index_view;
    Ref<gl::BufferViewMap<Vertex>> vertex_view;
};
} // namespace gnev