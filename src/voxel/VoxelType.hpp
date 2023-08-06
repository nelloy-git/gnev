#pragma once

namespace gnev {
    
enum class VoxelSide {
    Top,
    Bottom,
    Front,
    Back,
    Right,
    Left
};

class VoxelType {
public:
    VoxelType(bool transparent);
    virtual ~VoxelType();

    virtual bool is_transparent_for(VoxelSide side) const;
    virtual bool can_merge_with(VoxelSide size, const VoxelType& other) const;

private:
    const bool _trasparent;

};

}