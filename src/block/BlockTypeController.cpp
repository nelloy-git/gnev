#include "block/BlockTypeController.hpp"

using namespace gnev;

BlockTypeController::BlockTypeController(const std::shared_ptr<GladGLContext>& ctx) :
    _buffer(ctx){
}

BlockTypeController::~BlockTypeController(){   
}

GLint add(const std::filesystem::path& path){
    return -1;
    // if (path.compare())
}