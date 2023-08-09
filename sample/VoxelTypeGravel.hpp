#pragma once

#include <iostream>

#include "voxel/VoxelType.hpp"

#include "Vertex.hpp"

class VoxelTypeGravel : public gnev::VoxelType {
public:
    VoxelTypeGravel(GLint texture_id) : gnev::VoxelType(), _texture_id(texture_id){};
    virtual ~VoxelTypeGravel(){};

    virtual RectVertices get_rect_mesh(const gnev::VoxelRect& rectangle) const;

private:
    const GLint _texture_id;

    static const std::shared_ptr<GLuint[]>& _get_indices(gnev::VoxelSide side);
    static const std::shared_ptr<GLuint[]>& _get_vertices(gnev::VoxelSide side);

};
