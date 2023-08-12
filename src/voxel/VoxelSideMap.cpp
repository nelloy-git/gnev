#include "voxel/VoxelSideMap.hpp"

#include <iostream>
#include <stdexcept>

using namespace gnev;

VoxelSideMap::VoxelSideMap(VoxelChunk* owner, VoxelSide side, size_type size_x, size_type size_y, size_type size_z)
    : side(side),
      _owner(owner),
      _map(size_x, size_y, size_z)
{
}

VoxelSideMap::~VoxelSideMap()
{
}

const Array3d<std::shared_ptr<VoxelType>>& VoxelSideMap::get_map() const
{
    return _map;
}

const std::vector<std::shared_ptr<VoxelRectInfo>>& VoxelSideMap::get_rectangle_infos() const
{
    return _rectangle_infos;
}

void VoxelSideMap::build_map(const Array3d<std::shared_ptr<VoxelType>>& voxels)
{
    if (voxels.size_x != _map.size_x
        || voxels.size_y != _map.size_y
        || voxels.size_z != _map.size_z){
        throw std::logic_error("");
    }

    for (size_type x = 0; x < _map.size_x; ++x){
        for (size_type y = 0; y < _map.size_y; ++y){
            for (size_type z = 0; z < _map.size_z; ++z){
                auto p_voxel = voxels(x, y, z);
                _map(x, y, z) = _is_visible(voxels, p_voxel, x, y, z) ? p_voxel : nullptr;
            }
        }
    }
}

void VoxelSideMap::build_rectangles()
{
    _rectangle_infos.clear();
    Array3d<uint8_t> passed(_map.size_x, _map.size_y, _map.size_z, false);

    for (size_type x = 0; x < _map.size_x; ++x){
        for (size_type y = 0; y < _map.size_y; ++y){
            for (size_type z = 0; z < _map.size_z; ++z){
                auto& p_type = _map(x, y, z);
                if (!p_type || passed(x, y, z)){
                    continue;
                }
                
                auto rect = std::make_shared<VoxelRectInfo>();
                rect->type = p_type;
                rect->side = side;
                rect->x = x;
                rect->y = y;
                rect->z = z;

                _search_rect_width(passed, *rect);
                _search_rect_height(passed, *rect);
                _rectangle_infos.push_back(rect);
            }
        }
    }
}

void VoxelSideMap::_search_rect_width(Array3d<uint8_t>& passed, VoxelRectInfo& rect) const
{
    size_type width = 1;
    switch (side)
    {
    case VoxelSide::Top:
    case VoxelSide::Bottom:
        // z - width, x - height
        while(rect.z + width < _map.size_z){
            if (rect.type != _map(rect.x, rect.y, rect.z + width) || passed(rect.x, rect.y, rect.z + width)){
                break;
            }
            passed(rect.x, rect.y, rect.z + width) = true;
            ++width;
        }
        rect.size_z = width;
        break;

    case VoxelSide::Front:
    case VoxelSide::Back:
        // z - width, y - height
        while(rect.z + width < _map.size_z){
            if (rect.type != _map(rect.x, rect.y, rect.z + width) || passed(rect.x, rect.y, rect.z + width)){
                break;
            }
            passed(rect.x, rect.y, rect.z + width) = true;
            ++width;
        }
        rect.size_z = width;
        break;

    case VoxelSide::Left:
    case VoxelSide::Right:
        // x - width, y - height
        while(rect.x + width < _map.size_x){
            if (rect.type != _map(rect.x + width, rect.y, rect.z) || passed(rect.x + width, rect.y, rect.z)){
                break;
            }
            passed(rect.x + width, rect.y, rect.z) = true;
            ++width;
        }
        rect.size_x = width;
        break;
    
    default:
        throw std::logic_error("");
    }
}

void VoxelSideMap::_search_rect_height(Array3d<uint8_t>& passed, VoxelRectInfo& rect) const
{
    // std::cout << "_search_rect_height: " << static_cast<int>(side) << std::endl;
    size_type height = 1;
    switch (side)
    {
    case VoxelSide::Top:
    case VoxelSide::Bottom:
        // z - width, x - height
        while(rect.x + height + 1 < _map.size_x){
            bool same_type = true;
            for (size_type width = 0; width < rect.size_z; ++width){
                std::cout << "Checking height: " << height
                          << std::endl
                          << "\tx: " << rect.x + height << " y: " << rect.y << " z: " << rect.z + width
                          << std::endl
                          << "\tbase: " << rect.type.get() << " cmp: " << _map(rect.x + height, rect.y, rect.z + width).get()
                          << std::endl;                

                if (rect.type != _map(rect.x + height, rect.y, rect.z + width)
                    || passed(rect.x + height, rect.y, rect.z + width)){
                    same_type = false;
                    break;
                }
            }
            // std::cout << "Passed" << std::endl;

            if (!same_type){
                break;
            }

            for (size_type width = 0; width < rect.size_z; ++width){
                passed(rect.x + height, rect.y, rect.z + width) = true;
            }

            ++height;
        }
        rect.size_x = height;
        break;

    case VoxelSide::Front:
    case VoxelSide::Back:
        // z - width, y - height
        while(rect.y + height < _map.size_y){
            bool same_type = true;
            for (size_type width = 0; width < rect.size_z; ++width){
                if (rect.type != _map(rect.x, rect.y + height, rect.z + width)
                    || passed(rect.x, rect.y + height, rect.z + width)){
                    same_type = false;
                    break;
                }
            }

            if (!same_type){
                break;
            }

            for (size_type width = 0; width < rect.size_z; ++width){
                passed(rect.x, rect.y + height, rect.z + width) = true;
            }
            
            ++height;
        }
        rect.size_y = height;
        break;

    case VoxelSide::Left:
    case VoxelSide::Right:
        // x - width, y - height
        while(rect.y + height < _map.size_y){
            bool same_type = true;
            for (size_type width = 0; width < rect.size_x; ++width){
                if (rect.type != _map(rect.x + width, rect.y + height, rect.z)
                    || passed(rect.x + width, rect.y + height, rect.z)){
                    same_type = false;
                    break;
                }
            }

            if (!same_type){
                break;
            }

            for (size_type width = 0; width < rect.size_x; ++width){
                passed(rect.x + width, rect.y + height, rect.z) = true;
            }
            
            ++height;
        }
        rect.size_y = height;
        break;
    
    default:
        throw std::logic_error("");
    }
}

bool VoxelSideMap::_is_visible(const Array3d<std::shared_ptr<VoxelType>>& voxels,
                               const std::shared_ptr<const VoxelType>& type, 
                               size_type x, size_type y, size_type z) const
{
    if (!type){
        return false;
    }

    long int cx = x;
    long int cy = y;
    long int cz = z;

    switch (side)
    {
    using enum VoxelSide;
    case Top: cy += 1; break;
    case Bottom: cy -= 1; break;
    case Front: cx += 1; break;
    case Back: cx -= 1; break;
    case Left: cz += 1; break;
    case Right: cz -= 1; break;
    
    default:
        throw std::logic_error("");
    }

    // TODO check connected map
    if (cx < 0 || cx >= _map.size_x || cy < 0 || cy >= _map.size_y || cz < 0 || cz >= _map.size_z){
        return true;
    }

    auto& p_neighbour = voxels(cx, cy, cz);
    return !p_neighbour || type->is_visible(side, p_neighbour);
}