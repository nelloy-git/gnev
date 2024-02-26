#pragma once

#include "mesh/Mesh.hpp"
#include "util/InstanceLogger.hpp"

namespace gnev {

template <base::IsVertex Vertex>
class SubMesh {
public:
    SubMesh(const std::shared_ptr<Mesh<Vertex>>& mesh,
            unsigned index_count,
            unsigned vertex_count)
        : mesh{mesh}
        , indices{mesh->index_allocator->alloc(index_count)}
        , vertices{mesh->vertex_allocator->alloc(vertex_count)} {}

    SubMesh::~SubMesh() {
        mesh->index_allocator->free(indices);
        mesh->vertex_alloctor->free(vertices);
    }

    void setIndex(unsigned pos, unsigned value) {
        if (pos >= indices.size) {
            InstanceLogger{}.Log<ERROR, "pos >= indices.size ({} > {})">(pos,
                                                                         indices.size);
            return;
        }
        mesh->index_accessor->set((indices.offset + pos) * sizeof(unsigned),
                                  sizeof(unsigned),
                                  value);
    }

    void setVertex(unsigned pos, const Vertex& value) {
        if (pos >= vertices.size) {
            InstanceLogger{}.Log<ERROR, "pos >= vertices.size ({} > {})">(pos,
                                                                          vertices.size);
            return;
        }
        mesh->vertex_accessor->set((vertices.offset + pos) * sizeof(Vertex),
                                   sizeof(Vertex),
                                   value);
    }

private:
    std::shared_ptr<Mesh<Vertex>> mesh;
    gl::BufferAllocator::MemBlock indices;
    gl::BufferAllocator::MemBlock vertices;
};
} // namespace gnev