#include "gl/pipeline/Pipeline.hpp"

#include <memory>
#include <string_view>

namespace gnev::gl {

bool Pipeline::setShader(ShaderType type, const GLchar* const source, GLint length) {
    GNEV_LOG(L1, "setShader({}, {}, {})", type, (void*)(source), length);
    if (not source or length <= 0) {
        GNEV_LOG(L1, "Disabled shader stage {}", type);
        active_shader[type] = nullptr;
        return true;
    }

    auto shader = std::make_shared<Shader>(type);
    shader->setSource(1, &source, &length);
    shader->compile();

    bool status = shader->isCompileSucceed();
    if (status == GL_TRUE) {
        GNEV_LOG(L1,
                 "{} type: {} compiled with msg: {}",
                 fmtquill::to_string(*shader),
                 shader->getType(),
                 shader->getInfoLog());
        active_shader.at(type) = shader;
    } else {
        GNEV_LOG(WARNING,
                 "{} type: {} failed to compile with msg: {}",
                 fmtquill::to_string(*shader),
                 shader->getType(),
                 shader->getInfoLog());
        active_shader[type] = nullptr;
    }
    return status;
}

unsigned Pipeline::getAttribLoc(const std::string_view& name) const {
    return program.getAttributeLoc(name.data());
}

bool Pipeline::build() {
    for (auto& shader : active_shader) {
        if (shader) {
            program.attach(*shader);
        }
    }

    program.link();
    if (not program.isLinkSucceed()) {
        GNEV_LOG(WARNING, "Failed link program with msg: {}", program.getInfoLog());
        return false;
    }

    program.validate();
    if (not program.isValidateSucceed()){
        GNEV_LOG(WARNING, "Failed validate program with msg: {}", program.getInfoLog());
        return false;
    }

    return true;
}

void Pipeline::use() const {
    program.use();
}

} // namespace gnev::gl