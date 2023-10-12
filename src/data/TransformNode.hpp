#pragma once

#include <optional>
#include <type_traits>

#include "data/Transform.hpp"
#include "gl/buffer/CoherentStorage.hpp"

namespace gnev::data {

using TransformStorage = gnev::gl::buffer::CoherentStorage<Transform>;

class TransformNode {
public:
    TransformNode(TransformStorage& storage, Transform::Index index);
    virtual ~TransformNode();

    inline TransformStorage& getStorage() { return storage; }

    inline const TransformStorage& getStorage() const { return storage; }

    void setParent(const TransformNode& parent);
    void clearParent();
    std::optional<TransformNode> getParent() const;

    inline const glm::mat4& getMat() const { return data.getMat(); }

    inline void setPos(const glm::vec3& pos) { return data.setPos(pos); }

    inline const glm::vec3& getPos() const { return data.getPos(); }

    inline void setAngles(float yaw, float pitch, float roll) {
        return data.setAngles(yaw, pitch, roll);
    }

    inline void setYaw(float yaw) { return data.setYaw(yaw); }

    inline void setPitch(float pitch) { return data.setPitch(pitch); }

    inline void setRoll(float roll) { return data.setRoll(roll); }

    inline float getYaw() const { return data.getYaw(); }

    inline float getPitch() const { return data.getPitch(); }

    inline float getRoll() const { return data.getRoll(); }

    inline void setScale(const glm::vec3& scale) { return data.setScale(scale); }

    inline const glm::vec3& getScale() const { return data.getScale(); }

private:
    Transform::Index index;
    TransformStorage& storage;
    Transform& data;
};

} // namespace gnev::data