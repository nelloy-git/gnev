#include "gl/Sampler.hpp"

#include "gl/fmt/BitFlags.hpp"
#include "gl/fmt/Enum.hpp"
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
    GNEV_TRACE_L2("Sampler_{}::ctor()", handle());
}

Sampler::~Sampler() { GNEV_TRACE_L2("Sampler_{}::destr()", handle()); }

void Sampler::bind(GLuint unit) {
    Ctx::Get().glBindSampler(unit, handle());
    GNEV_TRACE_L2("Sampler_{}::bind({})", handle(), unit);
}

void Sampler::setWrapS(TextureWrapS wrap) {
    Ctx::Get().glSamplerParameteri(handle(),
                                   GL_TEXTURE_WRAP_S,
                                   static_cast<GLenum>(wrap));
    GNEV_TRACE_L2("Sampler_{}::setWrapS({})", handle(), wrap);
}

void Sampler::setWrapT(TextureWrapT wrap) {
    Ctx::Get().glSamplerParameteri(handle(),
                                   GL_TEXTURE_WRAP_T,
                                   static_cast<GLenum>(wrap));
    GNEV_TRACE_L2("Sampler_{}::setWrapT({})", handle(), wrap);
}

void Sampler::setMinFilter(TextureMinFilter filter) {
    Ctx::Get().glSamplerParameteri(handle(),
                                   GL_TEXTURE_MIN_FILTER,
                                   static_cast<GLenum>(filter));
    GNEV_TRACE_L2("Sampler_{}::setMinFilter({})", handle(), filter);
}

void Sampler::setMagFilter(TextureMagFilter filter) {
    Ctx::Get().glSamplerParameteri(handle(),
                                   GL_TEXTURE_MAG_FILTER,
                                   static_cast<GLenum>(filter));
    GNEV_TRACE_L2("Sampler_{}::setMagFilter({})", handle(), filter);
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