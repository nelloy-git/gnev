#pragma once

#include "gl/Texture.hpp"
#include "gl/Texture3dAccessor.hpp"

namespace gnev::gl {

class Texture3dArrayView {
public:
    using Changer = Texture3DAccessor::Changer;

    Texture3dArrayView(const Ref<Texture3DAccessor>& accessor);
    virtual ~Texture3dArrayView() = default;

    Ref<Texture3DAccessor> accessor;

    
};

} // namespace gnev::gl