#include "data/VertexInfo.hpp"

#include <stdexcept>

using namespace gnev;

VertexInfo::VertexInfo(const std::vector<AttributeInfo>& attributes) :
    size(_get_all_attributes_size(attributes)),
    attributes(attributes)
{
}

VertexInfo::~VertexInfo()
{
}

GLuint VertexInfo::_get_all_attributes_size(const std::vector<AttributeInfo>& attributes)
{
    GLuint size = 0;
    for (auto& attr : attributes){
        size += attr.size;
    }
    return size;
}