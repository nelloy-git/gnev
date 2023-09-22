#pragma once

#include "voxel/MeshBuilder.hpp"
#include "voxel/Region.hpp"

namespace gnev::voxel::mesh_builder {

class EXPORT SolidCube : public MeshBuilder {
public:
    using size_type = unsigned int;

    SolidCube();
    virtual ~SolidCube();

    virtual size_type get_indices_count() = 0;

    virtual MeshData build_mesh(const Region& region) const;

private:
};

} // namespace gnev::voxel::mesh_builder