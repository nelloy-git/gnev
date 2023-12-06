#pragma once

#include "transform/3d/TransformGL_3D.hpp"
#include "transform/base/Transform.hpp"

namespace gnev {

class EXPORT Transform_3D : public base::Transform<TransformGL_3D> {
    friend class TransformMap;

public:
    Transform_3D(WeakRef<base::TransformStorage<TransformGL_3D>> weak_storage);
    virtual ~Transform_3D() = default;

    std::optional<Ref<Transform_3D>> getParentRef() const;
    void setParentRef(const std::optional<Ref<Transform_3D>>& parent);

    glm::mat4 getMat() const;

    const glm::vec3& getPosition() const;
    void setPosition(const glm::vec3& position);

    const glm::vec3& getEuler() const;
    void setEuler(float yaw, float pitch, float roll);
    float getYaw() const;
    void setYaw(float yaw);
    float getPitch() const;
    void setPitch(float pitch);
    float getRoll() const;
    void setRoll(float roll);

    const glm::vec3& getScale() const;
    void setScale(const glm::vec3& scale);

private:
    glm::vec3 position{1.f};
    glm::vec3 euler{0.f};
    glm::vec3 scale{1.f};

    void apply();
};

} // namespace gnev

// namespace