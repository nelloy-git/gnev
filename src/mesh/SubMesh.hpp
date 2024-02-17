#pragma once

#include "mesh/Mesh.hpp"

namespace gnev {

template <base::IsVertex Vertex>
class SubMesh {
public:
    SubMesh(Ref<Mesh<Vertex>>& mesh, unsigned index_count, unsigned vertex_count)
        : mesh{mesh}
        , indices{mesh->reserveIndices(index_count).value()}
        , vertices{mesh->reserveVertices(vertex_count).value()} {}

    void setIndex(unsigned internal_n, unsigned value) {
        if (internal_n >= indices->count) {
            InstanceLogger{}
                .Log<ERROR, "internal_n >= indices->count ({} > {})">(internal_n,
                                                                      indices->count);
            return;
        }
        mesh->getIndexView()->set(indices->first + internal_n, vertices->first + value);
    }

    void setVertex(unsigned internal_n, const Vertex& value) {
        if (internal_n >= vertices->count) {
            InstanceLogger{}
                .Log<ERROR, "internal_n >= vertices->count ({} > {})">(internal_n,
                                                                       vertices->count);
            return;
        }
        mesh->getVertexView()->set(vertices->first + internal_n, value);
    }

private:
    Ref<Mesh<Vertex>> mesh;
    Ref<IndexGroup> indices;
    Ref<IndexGroup> vertices;
};
} // namespace gnev