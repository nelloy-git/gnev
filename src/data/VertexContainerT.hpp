#pragma once

#include "gl/GLBufferVectorCoherentT.hpp"
#include "data/Vertex.hpp"
#include "data/VertexContainer.hpp"

namespace gnev {

template<typename I, AttribInfo... A>
class EXPORT VertexContainerT : public VertexContainer {
    static_assert(std::is_same_v<I, GLubyte> || std::is_same_v<I, GLushort> || std::is_same_v<I, GLuint>, "Index type is not valid");
    static constexpr GLenum INDEX_TYPE = std::is_same_v<I, GLubyte> ? GL_UNSIGNED_BYTE
                                       : std::is_same_v<I, GLushort> ? GL_UNSIGNED_SHORT
                                       : std::is_same_v<I, GLuint> ? GL_UNSIGNED_INT : 0;

public:
    using index_t = typename I;
    using vertex_t = typename Vertex<A...>;

    explicit VertexContainerT(const VertexContainerT&);
    VertexContainerT(const std::shared_ptr<GladGLContext> &ctx);
    virtual ~VertexContainerT();

    GLBufferVectorCoherentT<index_t> indices();
    const GLBufferVectorCoherentT<index_t> indices() const;

    GLBufferVectorCoherentT<vertex_t> vertices();
    const GLBufferVectorCoherentT<vertex_t> vertices() const;

};

template<typename I, AttribInfo... A>
VertexContainerT<I, A...>::VertexContainerT(const VertexContainerT& reinterpret)
    : VertexContainer(reinterpret)
{
}

template<typename I, AttribInfo... A>
VertexContainerT<I, A...>::VertexContainerT(const std::shared_ptr<GladGLContext> &ctx)
    : VertexContainer(ctx, INDEX_TYPE, {A...})
{
}

template<typename I, AttribInfo... A>
VertexContainerT<I, A...>::~VertexContainerT()
{
}

template<typename I, AttribInfo... A>
GLBufferVectorCoherentT<typename VertexContainerT<I, A...>::index_t> VertexContainerT<I, A...>::indices()
{
    return GLBufferVectorCoherentT<typename VertexContainerT<I, A...>::index_t>(VertexContainer::indices());
}

template<typename I, AttribInfo... A>
const GLBufferVectorCoherentT<typename VertexContainerT<I, A...>::index_t> VertexContainerT<I, A...>::indices() const
{
    return GLBufferVectorCoherentT<typename VertexContainerT<I, A...>::index_t>(VertexContainer::indices());
}

template<typename I, AttribInfo... A>
GLBufferVectorCoherentT<typename VertexContainerT<I, A...>::vertex_t> VertexContainerT<I, A...>::vertices()
{
    return GLBufferVectorCoherentT<typename VertexContainerT<I, A...>::vertex_t>(VertexContainer::vertices());
}

template<typename I, AttribInfo... A>
const GLBufferVectorCoherentT<typename VertexContainerT<I, A...>::vertex_t> VertexContainerT<I, A...>::vertices() const
{
    return GLBufferVectorCoherentT<typename VertexContainerT<I, A...>::vertex_t>(VertexContainer::vertices());
}


}