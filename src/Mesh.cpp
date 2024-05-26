#include "Mesh.hpp"

#include "Ctx.hpp"

namespace gnev {

unsigned Mesh::getMaxAttributes() {
    static const unsigned MAX_ATTRIBUTES = []() {
        GLint max_attribs;
        Ctx::Get().glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_attribs);
        return max_attribs;
    }();
    return MAX_ATTRIBUTES;
}

} // namespace gnev