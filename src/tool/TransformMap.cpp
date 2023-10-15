#include "tool/TransformMap.hpp"

#include "gl/buffer/CoherentMap.hpp"
#include "gl/buffer/CoherentStorage.hpp"
#include "tool/Transform.hpp"

namespace gnev::tool {

TransformMap::TransformMap(Index capacity)
    : gl::buffer::CoherentMap<Transform::Index, Transform>(capacity) {
    auto& as_storage = static_cast<gl::buffer::CoherentStorage<Transform>&>(*this);
    for (Index i = 0; i < RESERVED_INDEX; ++i) {
        as_storage[i].node.index = i;
    }
}

TransformMap::~TransformMap() {}

} // namespace gnev::tool