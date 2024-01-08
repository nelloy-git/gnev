#pragma once

#include "gl/Handler.hpp"
#include "gl/enum/TextureMagFilter.hpp"
#include "gl/enum/TextureMinFilter.hpp"
#include "gl/enum/TextureWrapS.hpp"
#include "gl/enum/TextureWrapT.hpp"
#include "util/Ref.hpp"

namespace gnev::gl {

class EXPORT Sampler : public Handler {
public:
    static Ref<Sampler> MakeNearestRepeat();

    Sampler();
    Sampler(const Sampler&) = delete;
    Sampler(Sampler&&) = delete;
    virtual ~Sampler();

    void bind(GLuint unit);
    
    void setWrapS(TextureWrapS wrap);
    void setWrapT(TextureWrapT wrap);
    void setMinFilter(TextureMinFilter filter);
    void setMagFilter(TextureMagFilter filter);

private:
    static GLuint* createHandle();
    static void deleteHandle(GLuint* handle);
};

} // namespace gnev::gl