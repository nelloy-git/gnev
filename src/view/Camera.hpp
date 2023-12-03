// #pragma once

// #include "gl/buffer/ImmutableStorage.hpp"
// #include "glm/glm.hpp"

// namespace gnev {

// struct CameraBuffer {
//     glm::mat4 mat = glm::mat4(1.f);
//     glm::vec3 viewPos = {0, 0, 0};
//     glm::vec3 viewDir = {1, 0, 0};
// };

// class EXPORT Camera {
// public:
//     static constexpr GLbitfield STORAGE_FLAGS =
//         GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

//     Camera();
//     virtual ~Camera();

//     gl::buffer::ImmutableStorage<CameraBuffer> buffer;

//     void apply();
//     void applyLookAt(const glm::vec3& dst);

//     glm::vec3 pos = {0, 0, 0};
//     float yaw = 0;
//     float pitch = 0;
//     float roll = 0;

//     bool use_perspective = true;
//     float fov = float(2 * 3.14159265359 / 3);
//     float near_z = float(0.01);
//     float far_z = 10000;
//     int width = 1920;
//     int height = 1080;

// private:
//     CameraBuffer* _map;

//     glm::vec3 _forward = {1, 0, 0};
//     glm::vec3 _up = {0, 1, 0};
// };

// } // namespace gnev