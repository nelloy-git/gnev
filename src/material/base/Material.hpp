#pragma once

#include "util/Export.hpp"

namespace gnev {

class EXPORT Material {
public:
    Material() = default;
    virtual ~Material() = default;

    virtual unsigned int getIndex() const = 0;
};

} // namespace gnev::base