#pragma once

namespace gnev {

class Material;

enum class Side {
    Xpos,
    Xneg,
    Ypos,
    Yneg,
    Zpos,
    Zneg
};

class Voxel {
public:
    Voxel() = default;
    virtual ~Voxel() = default;

    virtual bool isNeigbourVisible(Side size) const = 0;
};

}