#pragma once

#include <optional>

#include "gl/texture/ImmutableStorage.hpp"

namespace gnev::gl::texture {

class EXPORT ImmutableIndexMap : public ImmutableStorage {
public:
    using CleanUp = std::optional<std::function<void(ImmutableIndexMap&, GLuint)>>;

    ImmutableIndexMap(GLuint levels,
                      GLuint width,
                      GLuint height,
                      GLuint capacity,
                      GLenum internal_format,
                      CleanUp clean_up = {});
    ImmutableIndexMap(const ImmutableIndexMap& other) = delete;
    ImmutableIndexMap(ImmutableIndexMap&& other) = default;
    virtual ~ImmutableIndexMap();

    ImmutableStorageIterator operator[](GLuint index);
    const ImmutableStorageIterator operator[](GLuint index) const;

    std::optional<GLuint> initUnusedIndex();
    void freeIndex(GLuint index);
    bool containsIndex(const GLuint& index) const;

private:
    CleanUp clean_up;
    std::unordered_set<GLuint> unused;
};

} // namespace gnev::gl::texture