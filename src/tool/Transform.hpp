#pragma once

#include <limits>

#include "glm/glm.hpp"
#include "util/Util.hpp"

namespace gnev::tool {

class EXPORT alignas(16) Transform final {
    friend class TransformMap;

public:
    using Index = unsigned int;
    static constexpr glm::vec3 UP = {0, 1, 0};
    static constexpr Index RESERVED_INDEX = std::numeric_limits<Index>::max();

    Transform();
    ~Transform() = default;

    Index getIndex() const;

    void setParent(Index parent);
    void setParent(const Transform& parent);
    void clearParent();
    Index getParentIndex() const;

    const glm::mat4& getMat() const;

    void setPos(const glm::vec3& pos);
    const glm::vec3& getPos() const;

    void setAngles(float yaw, float pitch, float roll);
    void setYaw(float yaw);
    void setPitch(float pitch);
    void setRoll(float roll);
    float getYaw() const;
    float getPitch() const;
    float getRoll() const;

    void setScale(const glm::vec3& value);
    const glm::vec3& getScale() const;

private:
    struct Node {
        Index index = RESERVED_INDEX;
        Index parent = RESERVED_INDEX;
    };

    Node node;
    glm::mat4 mat = glm::mat4{1.f};
    glm::vec3 pos = glm::vec3{0, 0, 0};
    glm::vec3 angles = glm::vec3{0, 0, 0};
    glm::vec3 scale = glm::vec3{1, 1, 1};

    void apply();
};

} // namespace gnev::tool

// namespace