#pragma once

#include "gl/data/VertexAttribute.hpp"

namespace gnev::gl {

struct VertexInfo {
    static constexpr unsigned int MAX_ATTRIBUTES = 16;
    using VertexAttributeInfoList = std::array<VertexAttributeInfo, MAX_ATTRIBUTES>;
    using VertexAttributeOffsetList = std::array<GLuint, MAX_ATTRIBUTES>;

    constexpr VertexInfo(const std::initializer_list<VertexAttributeInfo>& attributes);

    unsigned int size;
    unsigned int count;
    VertexAttributeOffsetList offsets;
    VertexAttributeInfoList attributes;

private:
    static constexpr unsigned int
    _init_size(const std::initializer_list<VertexAttributeInfo>& attributes);
    static constexpr unsigned int
    _init_count(const std::initializer_list<VertexAttributeInfo>& attributes);

    template <size_t... I>
    static constexpr VertexAttributeOffsetList
    _init_empty_offsets(const std::index_sequence<I...>&);
    static constexpr VertexAttributeOffsetList
    _init_offsets(const std::initializer_list<VertexAttributeInfo>& attributes);

    template <size_t... I>
    static constexpr VertexAttributeInfoList
    _init_empty_attributes(const std::index_sequence<I...>&);
    static constexpr VertexAttributeInfoList
    _init_attributes(const std::initializer_list<VertexAttributeInfo>& attributes);
};

constexpr VertexInfo::VertexInfo(const std::initializer_list<VertexAttributeInfo>& attributes)
    : size(_init_size(attributes))
    , count(_init_count(attributes))
    , attributes(_init_attributes(attributes))
    , offsets(_init_offsets(attributes)) {}

constexpr unsigned int
VertexInfo::_init_size(const std::initializer_list<VertexAttributeInfo>& attributes) {
    unsigned int size = 0;
    for (auto& attribute : attributes) {
        size += attribute.size;
    }
    return size;
}

constexpr unsigned int
VertexInfo::_init_count(const std::initializer_list<VertexAttributeInfo>& attributes) {
    return attributes.size();
}

template <size_t... I>
constexpr VertexInfo::VertexAttributeOffsetList
VertexInfo::_init_empty_offsets(const std::index_sequence<I...>&) {
    return VertexAttributeOffsetList{[](size_t) { return 0; }(I)...};
}

constexpr VertexInfo::VertexAttributeOffsetList
VertexInfo::_init_offsets(const std::initializer_list<VertexAttributeInfo>& attributes) {
    VertexAttributeOffsetList offsets(
        _init_empty_offsets(std::make_index_sequence<MAX_ATTRIBUTES>{}));

    size_t i = 0;
    GLuint offset = 0;
    for (auto& attribute : attributes) {
        offsets[i] = offset;
        offset += attribute.size;
        ++i;
    }

    return offsets;
}

template <size_t... I>
constexpr VertexInfo::VertexAttributeInfoList
VertexInfo::_init_empty_attributes(const std::index_sequence<I...>&) {
    return VertexAttributeInfoList{[](size_t) {
        return VertexAttributeInfo{1, GL_UNSIGNED_BYTE, false};
    }(I)...};
}

constexpr VertexInfo::VertexAttributeInfoList
VertexInfo::_init_attributes(const std::initializer_list<VertexAttributeInfo>& attributes) {
    VertexAttributeInfoList list(
        _init_empty_attributes(std::make_index_sequence<MAX_ATTRIBUTES>()));

    size_t i = 0;
    for (auto& attribute : attributes) {
        list[i] = attribute;
        ++i;
    }

    return list;
}

} // namespace gnev::gl::data