#include "VoxelTypeTest.hpp"

VoxelTypeTest::VoxelTypeTest(int material_id)
    : gnev::VoxelTypeSolidCube() {
    top_material_id = material_id;
    bottom_material_id = material_id;
    front_material_id = material_id;
    back_material_id = material_id;
    right_material_id = material_id;
    left_material_id = material_id;
}

VoxelTypeTest::~VoxelTypeTest() {}

gnev::VoxelRectMesh VoxelTypeTest::get_rect_mesh(const gnev::VoxelRectInfo& rect,
                                                 size_type base_index,
                                                 GLenum index_type,
                                                 const gnev::data::VertexInfo& vertex_info) {
    if (index_type != GL_UNSIGNED_INT) {
        return gnev::VoxelRectMesh{};
    }
    // TODO vertex struct check

    auto pos = get_position(rect);
    auto uv = get_uv(rect);
    auto id = get_material_id(rect);

    return gnev::VoxelRectMesh{
        .indices_count = 6,
        .indices_data =
            std::make_shared<gnev::VoxelTypeSolidCube::IndexArray>(get_indices(rect, base_index)),
        .vertices_count = 4,
        .vertices_data = std::make_shared<std::array<Vertex, 4>>(std::array<Vertex, 4>{
            Vertex{pos[0], uv[0], {id}},
            Vertex{pos[1], uv[1], {id}},
            Vertex{pos[2], uv[2], {id}},
            Vertex{pos[3], uv[3], {id}},
        }),
    };

    // std::cout << "Uploading VoxelTypeTest: " << std::endl;
    // for (int i = 0; i < 4; ++i){
    //     auto& cur = static_cast<const Vertex*>(mesh.vertices_data.get())[i];
    //     std::cout << "\t" << i << ": "
    //         << "{" << cur.get<0>().data[0] << ", " << cur.get<0>().data[1] << ", " <<
    //         cur.get<0>().data[2] << "}"
    //         << "{" << cur.get<1>().data[0] << ", " << cur.get<1>().data[1] << "}"
    //         << "{" << cur.get<2>().data[0] << "}"
    //         << std::endl;
    // }
}