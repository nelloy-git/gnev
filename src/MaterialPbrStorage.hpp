#pragma once

#include "MaterialPbr.hpp"
#include "gl/texture/IndexMapView.hpp"
#include "util/Export.hpp"

namespace gnev {

class MaterialPbrStorage {
public:
    // MaterialPbrStorage(std::unique_ptr<gl::IBufferAccessor>&& accessor, unsigned levels, unsigned width, unsigned height);
    // Dynamic storage
    MaterialPbrStorage(unsigned capacity, unsigned levels, unsigned width, unsigned height);
    virtual ~MaterialPbrStorage() = default;

    std::optional<MaterialPbr> create();

    const gl::Buffer& getDataBuffer() {
        return heap_buffer.getAccessor().getBuffer();
    }

    Ref<gl::texture::IndexMapView> getTexView(MaterialPbr::TexType tex_type) {
        return tex_view[static_cast<unsigned>(tex_type)];
    };

private:
    gl::HeapBuffer<MaterialPbr::GL> heap_buffer;
    std::array<Ref<gl::texture::IndexMapView>, MaterialPbr::TexTypeN> tex_view;

    static std::unique_ptr<gl::IBufferAccessor> initDynamicAccessor(unsigned capacity);
};

} // namespace gnev