#include "tool/TransformMap.hpp"

namespace gnev::tool {

TransformMap::TransformMap(Index capacity)
    : gl::buffer::CoherentMap<Index, Transform>(capacity) {
    auto& as_storage = static_cast<gl::buffer::CoherentStorage<Transform>&>(*this);
    for (Index i = 0; i < RESERVED_INDEX; ++i) {
        as_storage[i].node.index = i;
    }
}

TransformMap::~TransformMap() {}

} // namespace gnev::tool