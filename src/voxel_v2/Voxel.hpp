#pragma once

#include "voxel_v2/MeshProvider.hpp"

namespace gnev::voxel_v2 {

template <data::IsMesh M>
class Voxel : public MeshProvider<M> {

};

}
