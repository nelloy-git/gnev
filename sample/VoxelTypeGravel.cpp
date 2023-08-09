#include "VoxelTypeGravel.hpp"

VoxelTypeGravel::RectVertices VoxelTypeGravel::get_rect_mesh(const gnev::VoxelRect& rectangle) const
{
    if (rectangle.user_data.side != gnev::VoxelSide::Top){
        return RectVertices{};
    }

    float x = rectangle.x;
    float y = rectangle.y;
    float z = rectangle.z;
    float sx = rectangle.size_x;
    float sy = rectangle.size_y;
    float sz = rectangle.size_z;
    float uv_width = rectangle.size_x != 0 ? rectangle.size_x : rectangle.size_y;
    float uv_height = rectangle.size_y != 0 ? rectangle.size_y : rectangle.size_z;



    RectVertices mesh;
    mesh.indices_count = 6;
    mesh.indices_type = GL_UNSIGNED_INT;
    std::shared_ptr<GLuint[6]> indices(new GLuint[6]{
        0, 1, 2, 2, 3, 0
    });
    mesh.indices_data = indices;
    mesh.vertices_count = 4;
    mesh.vertex_size = Vertex::size;
    std::shared_ptr<Vertex[4]> vertices(new Vertex[4]{
        {{x, y + 1, z},           {0, 0},                {_texture_id}},
        {{x, y + 1, z + sz},      {0, uv_height},        {_texture_id}},
        {{x + sx, y + 1, z + sz}, {uv_width, uv_height}, {_texture_id}},
        {{x + sx, y + 1, z},      {uv_width, 0},         {_texture_id}},
    });
    mesh.vertices_data = vertices;
    
    std::cout << "Uploading VoxelTypeGravel: " << std::endl;
    for (int i = 0; i < 4; ++i){
        auto& cur = static_cast<const Vertex*>(mesh.vertices_data.get())[i];
        std::cout << "\t" << i << ": "
            << "{" << cur.get_attrib<0>().data[0] << ", " << cur.get_attrib<0>().data[1] << ", " << cur.get_attrib<0>().data[2] << "}"
            << "{" << cur.get_attrib<1>().data[0] << ", " << cur.get_attrib<1>().data[1] << "}"
            << "{" << cur.get_attrib<2>().data[0] << "}"
            << std::endl;
    }

    return mesh;
}