#pragma once

#include "util/Util.hpp"

namespace gnev {

class EXPORT MeshBuilder {
public:
    MeshBuilder();
    virtual ~MeshBuilder();

    virtual void build_mesh(void* dst_indices, void* dst_vertices) = 0;


};

MeshBuilder::MeshBuilder()
{
}

inline MeshBuilder::~MeshBuilder()
{
}

}