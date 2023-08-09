#include "voxel/VoxelChunk.hpp"

#include <iostream>

#include "Vertex.hpp"

using namespace gnev;

VoxelChunk::VoxelChunk(const std::shared_ptr<GladGLContext> &ctx,
                       GLenum index_type, const std::initializer_list<AttribInfo>& vertex_info,
                       size_type size_x, size_type size_y, size_type size_z)
    : _mesh(ctx, index_type, vertex_info),
      _voxels(size_x, size_y, size_z, nullptr),
      _top(VoxelSide::Top, size_x, size_y, size_z)
{
}

VoxelChunk::~VoxelChunk()
{
}

VertexContainer& VoxelChunk::mesh()
{
    return _mesh;
}

void VoxelChunk::set(const std::shared_ptr<VoxelType>& voxel_type, size_type x, size_type y, size_type z)
{
    _voxels(x, y, z) = voxel_type;
}

void VoxelChunk::apply_mesh()
{
    _mesh.indices().clear();
    _mesh.vertices().clear();

    _top.build_map(_voxels);
    _top.build_rectangles();

    size_type base_vertex = 0;
    auto& top_rects = _top.get_rectangles();

    std::cout << "Rectangles found: " << top_rects.size() << std::endl;

    int i = 0;
    for (auto& rect : top_rects){
        std::cout << "N: " << i
                  << " x: " << rect.x
                  << " y: " << rect.y
                  << " z: " << rect.z
                  << " size_x: " << rect.size_x
                  << " size_y: " << rect.size_y
                  << " size_z: " << rect.size_z
                  << std::endl;
        ++i;

        auto& type = *rect.user_data.type;
        auto voxel_mesh = type.get_rect_mesh(rect);

        if (voxel_mesh.indices_count == 0){
            continue;
        }

        if (_mesh.index_type != voxel_mesh.indices_type
            || _mesh.vertex_size != voxel_mesh.vertex_size){
            throw std::logic_error("");
        }

        void* indices_with_offset = malloc(voxel_mesh.indices_count * _mesh.index_size);
        switch (_mesh.index_type)
        {
        case GL_UNSIGNED_BYTE:
            for (size_type i = 0; i < voxel_mesh.indices_count; ++i){
                static_cast<GLubyte*>(indices_with_offset)[i] = base_vertex + static_cast<const GLubyte*>(voxel_mesh.indices_data.get())[i];
            }
            break;
        case GL_UNSIGNED_SHORT:
            for (size_type i = 0; i < voxel_mesh.indices_count; ++i){
                static_cast<GLushort*>(indices_with_offset)[i] = base_vertex + static_cast<const GLushort*>(voxel_mesh.indices_data.get())[i];
            }
            break;
        case GL_UNSIGNED_INT:
            for (size_type i = 0; i < voxel_mesh.indices_count; ++i){
                static_cast<GLuint*>(indices_with_offset)[i] = base_vertex + static_cast<const GLuint*>(voxel_mesh.indices_data.get())[i];
            }
            break;
        
        default:
            break;
        }

        std::cout << "Uploading VoxelChunk: " << std::endl;
        std::shared_ptr<const ::Vertex[4]> vert = std::static_pointer_cast<const ::Vertex[4]>(voxel_mesh.vertices_data);
        for (int i = 0; i < 4; ++i){
            auto& cur = vert[i];
            std::cout << "\t" << i << ": "
                << "{" << cur.get_attrib<0>().data[0] << ", " << cur.get_attrib<0>().data[1] << ", " << cur.get_attrib<0>().data[2] << "}"
                << "{" << cur.get_attrib<1>().data[0] << ", " << cur.get_attrib<1>().data[1] << "}"
                << "{" << cur.get_attrib<2>().data[0] << "}"
                << std::endl;
        }

        _mesh.indices().push_back(voxel_mesh.indices_count * _mesh.index_size, indices_with_offset);
        _mesh.vertices().push_back(voxel_mesh.vertices_count * voxel_mesh.vertex_size, voxel_mesh.vertices_data.get());

        base_vertex += voxel_mesh.vertices_count;
        free(indices_with_offset);
    }
}