#pragma once

#include <unordered_set>
#include "gl/buffer/CoherentMap.hpp"
#include "tool/Transform.hpp"

namespace gnev::tool {

class TransformMap : public gl::buffer::CoherentMap<Transform::Index, Transform> {
public:
    using Index = Transform::Index;
    static constexpr Index RESERVED_INDEX = Transform::RESERVED_INDEX;

    TransformMap(Index capacity);
    virtual ~TransformMap();

};

} // namespace gnev::tool