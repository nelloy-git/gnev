#pragma once

#include "glm/glm.hpp"
#include "util/Export.hpp"

namespace gnev {

struct EXPORT CameraGLdata {
    static constexpr unsigned int InvalidIndex = std::numeric_limits<unsigned int>::max();

    alignas(16) glm::vec3 position = {0, 0, 0};
    alignas(16) glm::vec3 direction = {1, 0, 0};
    alignas(16) glm::vec3 top = {0, 1, 0};

    alignas(16) struct {
        glm::uint view = InvalidIndex;
        glm::uint proj = InvalidIndex;
    } mats;
};

static_assert(offsetof(CameraGLdata, position) == 0,
              "offsetof(CameraGLdata, position) != 0");
static_assert(offsetof(CameraGLdata, direction) == 16,
              "offsetof(CameraGLdata, direction) != 16");
static_assert(offsetof(CameraGLdata, top) == 32, "offsetof(CameraGLdata, top) != 32");
static_assert(offsetof(CameraGLdata, mats) == 48, "offsetof(CameraGLdata, mats) != 48");

} // namespace gnev