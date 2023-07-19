#pragma once

#include <vector>

#include "data/AttributeInfo.hpp"

namespace gnev {

class EXPORT VertexInfo {
public:
    VertexInfo(const std::vector<AttributeInfo>& attributes);
    virtual ~VertexInfo();

    const GLuint size;
    const std::vector<AttributeInfo> attributes;

private:
    static GLuint _get_all_attributes_size(const std::vector<AttributeInfo>& attributes);

};

}