#pragma once

#include <optional>

#include "gl/texture/TexStorage.hpp"

namespace gnev::gl {

class EXPORT TexStorageIndexMap : public TexStorage {
public:
    using CleanUp = std::optional<std::function<void(TexStorageIndexMap&, GLuint)>>;

    TexStorageIndexMap(GLuint levels,
                       GLuint width,
                       GLuint height,
                       GLuint capacity,
                       GLenum internal_format,
                       CleanUp clean_up = std::nullopt);
    TexStorageIndexMap(const TexStorageIndexMap& other) = delete;
    TexStorageIndexMap(TexStorageIndexMap&& other) = default;
    virtual ~TexStorageIndexMap();

    TexStorageIterator at(GLuint index);
    const TexStorageIterator at(GLuint index) const;

    std::optional<GLuint> initUnusedIndex();
    void freeIndex(GLuint index);
    bool containsIndex(const GLuint& index) const;

private:
    CleanUp clean_up;
    std::unordered_set<GLuint> unused;
};

} // namespace gnev::gl