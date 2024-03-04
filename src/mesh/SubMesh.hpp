#pragma once

#include "mesh/base/Vertex.hpp"
#include "gl/buffer/HeapBufferRange.hpp"
#include "util/InstanceLogger.hpp"

namespace gnev {

template <base::IsVertex Vertex>
class SubMesh {
public:
    SubMesh(const gl::HeapBufferRange<unsigned>& indices,
            const gl::HeapBufferRange<Vertex>& vertices)
        : indices{indices}
        , vertices{vertices} {}

    virtual ~SubMesh() = default;

    void setIndex(unsigned pos, unsigned value) {
        indices.set(pos, &value);
    }

    void setVertex(unsigned pos, const Vertex& value) {
        vertices.set(pos, &value);
    }

private:
    gl::HeapBufferRange<unsigned> indices;
    gl::HeapBufferRange<Vertex> vertices;
};
} // namespace gnev