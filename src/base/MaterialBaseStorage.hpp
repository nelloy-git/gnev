#pragma once

#include <array>

#include "gl/buffer/IBufferAccessor.hpp"
#include "gl/texture/IndexMapViewElem.hpp"

namespace gnev {

template <typename T, unsigned TexN>
class MaterialBaseStorage {
public:
    MaterialBaseStorage(const std::unique_ptr<gl::IBufferAccessor>&& data_accessor,
                        const std::array<std::shared_ptr<gl::texture::IndexMapView>, TexN> tex_accessors);

private:
    

};

} // namespace gnev