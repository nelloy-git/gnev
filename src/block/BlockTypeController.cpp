#include "block/BlockTypeController.hpp"

#include <iostream>
#include <fstream>
#include <thread>

#include "nlohmann/json.hpp"

using namespace gnev;
using json = nlohmann::json;

const std::unordered_map<std::string, BlockSide> BlockTypeController::SIDE_MAP = {
    {"Top", BlockSide::Top},
    {"Bottom", BlockSide::Bottom},
    {"Front", BlockSide::Front},
    {"Left", BlockSide::Left},
    {"Right", BlockSide::Right},
    {"Back", BlockSide::Back},
};

BlockTypeController::BlockTypeController(const std::shared_ptr<GladGLContext>& ctx) :
    // _types_buffer(ctx),
    _textures_controller(ctx, TEX_WIDTH, TEX_HEIGHT){
}

BlockTypeController::~BlockTypeController(){   
}

GLint BlockTypeController::add(const std::filesystem::path& path){
    BlockTypeData block_type;

    std::ifstream json_file(path);
    json data = json::parse(json_file);
    
    auto& texture = data["Texture"];
    if (!texture.is_object()){
        return -1;
    }

    for (auto it = texture.begin(); it != texture.end(); ++it){
        auto found_side = SIDE_MAP.find(it.key());
        if (found_side == SIDE_MAP.end()){
            continue;
        }
        auto side = found_side->second;

        if (!it.value().is_string()){
            continue;
        }

        std::filesystem::path tex_path(std::string(it.value()));
        GLint tex_id = _textures_controller.load(tex_path);
        std::cout << "loaded id = " << tex_id << "\t" << "side: " << found_side->first.c_str() << "\t" << tex_path.string().c_str() << std::endl;

        block_type.texture[static_cast<GLuint>(side)] = tex_id;
    }

    // _types_buffer.push_back(block_type);
    return id++;
}

// GLBufferVectorT<BlockTypeController::BlockTypeData>& BlockTypeController::types_buffer(){
//     return _types_buffer;
// }

// const GLBufferVectorT<BlockTypeController::BlockTypeData>& BlockTypeController::types_buffer() const {
//     return _types_buffer;
// }

const TextureController& BlockTypeController::textures_controller() const {
    return _textures_controller;
}