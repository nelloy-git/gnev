#pragma once

#include <limits>

#include "glm/glm.hpp"
#include "util/Util.hpp"

namespace gnev::tool {

class EXPORT alignas(16) Material final {
    friend class MaterialMap;

public:
    using Index = unsigned int;
    static constexpr Index RESERVED_INDEX = std::numeric_limits<Index>::max();

    Material();
    ~Material() = default;

    Index getIndex() const;

    void setDiffuse(Index diffuse);
    Index getDiffuse() const;

    void setNormal(Index normal);
    Index getNormal() const;

    void setSpecular(Index specular);
    Index getSpecular() const;

    void setDiffuseMult(const glm::vec4& diffuse);
    glm::vec4 getDiffuseMult() const;

    void setNormalMult(const glm::vec4& normal);
    glm::vec4 getNormalMult() const;

    void setSpecularMult(const glm::vec4& specular);
    glm::vec4 getSpecularMult() const;

    void setDiffuseAdd(const glm::vec4& diffuse);
    glm::vec4 getDiffuseAdd() const;

    void setNormalAdd(const glm::vec4& normal);
    glm::vec4 getNormalAdd() const;

    void setSpecularAdd(const glm::vec4& specular);
    glm::vec4 getSpecularAdd() const;


private:
    struct Indices {
        Index self_index = RESERVED_INDEX;
        Index diffuse = RESERVED_INDEX;
        Index normal = RESERVED_INDEX;
        Index specular = RESERVED_INDEX;
    };

    Indices indices;
    glm::vec4 diffuse_mult = {1, 1, 1, 1};
    glm::vec4 diffuse_add = {0, 0, 0, 0};
    glm::vec4 normal_mult = {1, 1, 1, 1};
    glm::vec4 normal_add = {0, 0, 0, 0};
    glm::vec4 specular_mult = {1, 1, 1, 1};
    glm::vec4 specular_add = {0, 0, 0, 0};

};

} // namespace gnev