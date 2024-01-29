#include "gl/Sampler.hpp"

#include "gl/fmt/BitFlags.hpp"
#include "gl/fmt/Enum.hpp"
#include "gl/fmt/HandlerLog.hpp"
#include "util/Log.hpp"

namespace gnev::gl {

Ref<Sampler> Sampler::MakeNearestRepeat() {
    auto sampler = MakeSharable<Sampler>();
    sampler->setWrapS(TextureWrapS::REPEAT);
    sampler->setWrapT(TextureWrapT::REPEAT);
    sampler->setMinFilter(TextureMinFilter::NEAREST);
    sampler->setMagFilter(TextureMagFilter::NEAREST);
    return sampler;
}

Sampler::Sampler()
    : Handler(createHandle(), &deleteHandle) {
    Log()->L2();
}

Sampler::~Sampler() { Log()->L2(); }

void Sampler::bind(GLuint unit) {
    Log()->L2(unit);
    Ctx::Get().glBindSampler(unit, handle());
}

void Sampler::setWrapS(TextureWrapS wrap) {
    Log()->L2(wrap);
    Ctx::Get().glSamplerParameteri(handle(),
                                   GL_TEXTURE_WRAP_S,
                                   static_cast<GLenum>(wrap));
}

void Sampler::setWrapT(TextureWrapT wrap) {
    Log()->L2(wrap);
    Ctx::Get().glSamplerParameteri(handle(),
                                   GL_TEXTURE_WRAP_T,
                                   static_cast<GLenum>(wrap));
}

void Sampler::setMinFilter(TextureMinFilter filter) {
    Log()->L2(filter);
    Ctx::Get().glSamplerParameteri(handle(),
                                   GL_TEXTURE_MIN_FILTER,
                                   static_cast<GLenum>(filter));
}

void Sampler::setMagFilter(TextureMagFilter filter) {
    Log()->L2(filter);
    Ctx::Get().glSamplerParameteri(handle(),
                                   GL_TEXTURE_MAG_FILTER,
                                   static_cast<GLenum>(filter));
}

GLuint* Sampler::createHandle() {
    auto handle = new GLuint(0);
    Ctx::Get().glCreateSamplers(1, handle);
    return handle;
}

void Sampler::deleteHandle(GLuint* handle) {
    Ctx::Get().glDeleteSamplers(1, handle);
    delete handle;
}

} // namespace gnev::gl