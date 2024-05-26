#include "gl/enum/ShaderType.hpp"

namespace gnev::gl {

std::string_view format_as(ShaderType val){
    switch (val) {
        using enum ShaderType;
        case COMPUTE_SHADER: return "GL_COMPUTE_SHADER";
        case VERTEX_SHADER: return "GL_VERTEX_SHADER";
        case TESS_CONTROL_SHADER: return "GL_TESS_CONTROL_SHADER";
        case TESS_EVALUATION_SHADER: return "GL_TESS_EVALUATION_SHADER";
        case GEOMETRY_SHADER: return "GL_GEOMETRY_SHADER";
        case FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
        default: return "UNKNOWN";
    }
}

}