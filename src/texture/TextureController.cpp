// #include "texture/TextureController.hpp"

// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

// using namespace gnev;

// TextureController::TextureController(const std::shared_ptr<GladGLContext>& ctx) :
//     _ctx(ctx){
// }

// TextureController::~TextureController(){

// }

// bool TextureController::init(GLsizei width, GLsizei height){
//     _chunks.emplace_back(_ctx);
//     _chunks.back().init(width, height);
// }

// GLint TextureController::load(const std::filesystem::path& path){
//     auto wstr = path.generic_wstring();
//     auto found = _path_map.find(wstr);
//     if (found != _path_map.end()){
//         return found->second;
//     }

//     stbi_set_flip_vertically_on_load(true);
//     int width, height, channels;

//     auto img = stbi_load("../3rdparty/minecraft_textures/dirt.png", &width, &height, &channels, 4);
//     auto id = _chunks.back().add(img);

//     if (id < 0){
//         auto img_w = _chunks.back().width();
//         auto img_h = _chunks.back().height();

//         _chunks.emplace_back(_ctx);
//         _chunks.back().init(width, height);
//         id = _chunks.back().add(img);
//     }
//     stbi_image_free(img);

    
// }