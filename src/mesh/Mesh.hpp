#pragma once

#include "gl/buffer/IndexMapViewElem.hpp"
#include "mesh/base/Vertex.hpp"

namespace gnev {

template <typename Index, base::IsVertex Vertex>
class Mesh {
public:
    using IndexView = gl::buffer::IndexMapView<Index>;
    
};
} // namespace gnev