#include "gl/Sampler.hpp"

#include "gl/fmt/BitFlags.hpp"
#include "gl/fmt/Enum.hpp"
#include "gl/logger/HandlerLogger.hpp"

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
    Log()->Func();
}

Sampler::~Sampler() { Log()->Func(); }

void Sampler::bind(GLuint unit) {
    Log()->Func(unit);
    Ctx::Get().glBindSampler(unit, handle());
}

void Sampler::setWrapS(TextureWrapS wrap) {
    Log()->Func(wrap);
    Ctx::Get().glSamplerParameteri(handle(),
                                   GL_TEXTURE_WRAP_S,
                                   static_cast<GLenum>(wrap));
}

void Sampler::setWrapT(TextureWrapT wrap) {
    Log()->Func(wrap);
    Ctx::Get().glSamplerParameteri(handle(),
                                   GL_TEXTURE_WRAP_T,
                                   static_cast<GLenum>(wrap));
}

void Sampler::setMinFilter(TextureMinFilter filter) {
    Log()->Func(filter);
    Ctx::Get().glSamplerParameteri(handle(),
                                   GL_TEXTURE_MIN_FILTER,
                                   static_cast<GLenum>(filter));
}

void Sampler::setMagFilter(TextureMagFilter filter) {
    Log()->Func(filter);
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