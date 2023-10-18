#include "tool/MaterialData.hpp"

namespace gnev::tool {

MaterialData::MaterialData() {}

MaterialData::Index MaterialData::getIndex() const { return indices.self_index; }

void MaterialData::setDiffuse(Index diffuse) { indices.diffuse = diffuse; }

MaterialData::Index MaterialData::getDiffuse() const { return indices.diffuse; }

void MaterialData::setNormal(Index normal) { indices.normal = normal; }

MaterialData::Index MaterialData::getNormal() const { return indices.normal; }

void MaterialData::setSpecular(Index specular) { indices.specular = specular; }

MaterialData::Index MaterialData::getSpecular() const { return indices.specular; }

void MaterialData::setDiffuseMult(const glm::vec4& diffuse) { diffuse_mult = diffuse; }

glm::vec4 MaterialData::getDiffuseMult() const { return diffuse_mult; }

void MaterialData::setNormalMult(const glm::vec4& normal) { normal_mult = normal; }

glm::vec4 MaterialData::getNormalMult() const { return normal_mult; }

void MaterialData::setSpecularMult(const glm::vec4& normal) { normal_mult = normal; }

glm::vec4 MaterialData::getSpecularMult() const { return normal_mult; }

void MaterialData::setDiffuseAdd(const glm::vec4& diffuse) { diffuse_add = diffuse; }

glm::vec4 MaterialData::getDiffuseAdd() const { return diffuse_add; }

void MaterialData::setNormalAdd(const glm::vec4& normal) { normal_add = normal; }

glm::vec4 MaterialData::getNormalAdd() const { return normal_add; }

void MaterialData::setSpecularAdd(const glm::vec4& normal) { normal_add = normal; }

glm::vec4 MaterialData::getSpecularAdd() const { return normal_add; }

} // namespace gnev::tool