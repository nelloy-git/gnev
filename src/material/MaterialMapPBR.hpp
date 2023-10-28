#pragma once

#include "tool/MaterialMap.hpp"

namespace gnev {

class MaterialMapPBR {
public:
    MaterialMapPBR(std::size_t capacity);
    virtual ~MaterialMapPBR();

private:
    tool::MaterialStorage<5> map;
};
} // namespace gnev