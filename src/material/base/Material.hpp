#pragma once

#include "gl/buffer/CoherentIndexMap.hpp"
#include "gl/texture/ImmutableIndexMap.hpp"

namespace gnev::base {

template <IsTriviallyCopyable T, unsigned int TexN>
class EXPORT Material {
public:
    using Index = unsigned int;
    using Data = T;
    using DataStorage = gl::buffer::CoherentIndexMap<Data>;
    using TextureStorage = std::array<gl::texture::ImmutableIndexMap, TexN>;
    using TextureView = gl::texture::ImmutableStorageIterator;

    Material(const std::shared_ptr<DataStorage>& p_data,
             const std::shared_ptr<TextureStorage>& p_tex)
        : p_data(p_data)
        , p_tex(p_tex)
        , index(initIndex()) {}

    virtual ~Data() { p_data->freeIndex(index); }

    inline Data& getData() { return p_data->operator[](index); };

    inline const Data& getData() const { return p_data->operator[](index); };

    virtual std::optional<TextureView> getTexture(Index tex_n) = 0;

    virtual std::optional<const TextureView> getTexture(Index tex_n) const = 0;

private:
    std::shared_ptr<DataStorage> p_data;
    std::shared_ptr<TextureStorage> p_tex;
    Index index;

    Index initIndex() const {
        auto index_opt = p_data->initUnusedIndex();
        if (!index_opt.has_value()) {
            throw std::out_of_range("");
        }
        return index_opt.value();
    };
};

} // namespace gnev::base