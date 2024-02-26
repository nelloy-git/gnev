#include "Mat4x4.hpp"

namespace gnev {

Mat4x4::Mat4x4(const gl::HeapBufferRange<GL>& range) : gl{range}{}

unsigned Mat4x4::getIndex() const {
    return gl.getOffset();
}

glm::mat4x4 Mat4x4::get() const {
    glm::mat4x4 mat;
    gl.get(0, &mat);
    return mat;
}

void Mat4x4::set(const glm::mat4x4& mat) {
    gl.set(0, &mat);
}

}