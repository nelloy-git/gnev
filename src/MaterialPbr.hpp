#pragma once

#include "gl/buffer/HeapBufferRange.hpp"
#include "gl/texture/IndexMapViewElem.hpp"
#include "glm/vec4.hpp"
#include "magic_enum/magic_enum.hpp"

namespace gnev {

class EXPORT MaterialPbr {
public:
    using TexPtr = std::shared_ptr<gl::texture::WeakIndexMapViewElem>;

    enum class TexType : unsigned {
        Albedo = 0,
        Normal,
        Metallic,
        Roughness,
        // AmbientOcclusion,
    };
    static constexpr unsigned TexTypeN = magic_enum::enum_count<TexType>();

    struct GL final {
        static constexpr unsigned int InvalidIndex =
            std::numeric_limits<unsigned int>::max();

        alignas(16) std::array<unsigned, TexTypeN> index;
        alignas(16) std::array<glm::vec4, TexTypeN> offset;
        alignas(16) std::array<glm::vec4, TexTypeN> multiplier;
    };

    MaterialPbr(const gl::HeapBufferRange<GL>& data)
        : heap_range{data} {};

    void setTex(TexType type, const TexPtr& texture) {
        unsigned type_n = static_cast<unsigned>(type);
        auto index = texture ? texture->getIndex() : GL::InvalidIndex;
        heap_range.setSub(0, &index, offsetof(GL, index) + type_n * sizeof(unsigned));
        textures[type_n] = texture;
    };

private:
    gl::HeapBufferRange<GL> heap_range;
    std::array<TexPtr, TexTypeN> textures;
};

} // namespace gnev