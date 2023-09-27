#pragma once

#include "data/Index.hpp"
#include "data/Vertex.hpp"
#include "gl/BufferVector.hpp"
#include "gl/VertexArray.hpp"

namespace gnev::data {

template <IsIndex I, IsVertex V>
class Mesh {
public:
    static constexpr GLenum Index = IndexEnum<I>;

    using index_type = I;
    using vertex_type = V;

    Mesh(const gl::GladCtx& ctx)
        : _vao(ctx)
        , _indices(ctx, 0, nullptr)
        , _vertices(ctx, 0, nullptr) {
        _vao.glVertexArrayElementBuffer(_indices.handle());
        _vao.glVertexArrayVertexBuffer(0, _vertices.handle(), 0, sizeof(vertex_type));
    };

    virtual ~Mesh(){};

    void bindAttribute(GLint shader_loc, GLint attribute_loc) {
        static constexpr VertexInfo vertex_info = vertex_type::info;
        if (shader_loc < 0 || attribute_loc < 0 || attribute_loc >= vertex_info.count) {
            throw std::out_of_range("");
        }
        const auto attr_info = vertex_info.attributes[attribute_loc];
        _vao.glVertexArrayAttribBinding(shader_loc, 0);
        _vao.glVertexArrayAttribFormat(shader_loc,
                                       attr_info.elements,
                                       attr_info.type,
                                       attr_info.normalized,
                                       vertex_info.offsets[attribute_loc]);
        _vao.glEnableVertexArrayAttrib(shader_loc);
    }

    inline gl::VertexArray vao() const { return _vao; }

    inline gl::BufferVector<I> indices() const { return _indices; };

    inline gl::BufferVector<V> vertices() const { return _vertices; };

private:
    gl::VertexArray _vao;
    gl::BufferVector<I> _indices;
    gl::BufferVector<V> _vertices;
};

namespace details {

template <typename derived>
struct is_Mesh {
    template <IsIndex I, IsVertex V>
    static constexpr std::true_type test(const Mesh<I, V>*);
    static constexpr std::false_type test(...);
    static constexpr bool value = decltype(test(std::declval<derived*>()))::value;
};

} // namespace details

template <typename T>
concept IsMesh = details::is_Mesh<T>::value;

} // namespace gnev::data