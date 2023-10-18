#include "tool/Material.hpp"

namespace gnev::tool {

Material::Material() {}

Material::Index Material::getIndex() const { return indices.self_index; }

void Material::setDiffuse(Index diffuse) { indices.diffuse = diffuse; }

Material::Index Material::getDiffuse() const { return indices.diffuse; }

void Material::setNormal(Index normal) { indices.normal = normal; }

Material::Index Material::getNormal() const { return indices.normal; }

void Material::setSpecular(Index specular) { indices.specular = specular; }

Material::Index Material::getSpecular() const { return indices.specular; }

void Material::setDiffuseMult(const glm::vec4& diffuse) { diffuse_mult = diffuse; }

glm::vec4 Material::getDiffuseMult() const { return diffuse_mult; }

void Material::setNormalMult(const glm::vec4& normal) { normal_mult = normal; }

glm::vec4 Material::getNormalMult() const { return normal_mult; }

void Material::setSpecularMult(const glm::vec4& normal) { normal_mult = normal; }

glm::vec4 Material::getSpecularMult() const { return normal_mult; }

void Material::setDiffuseAdd(const glm::vec4& diffuse) { diffuse_add = diffuse; }

glm::vec4 Material::getDiffuseAdd() const { return diffuse_add; }

void Material::setNormalAdd(const glm::vec4& normal) { normal_add = normal; }

glm::vec4 Material::getNormalAdd() const { return normal_add; }

void Material::setSpecularAdd(const glm::vec4& normal) { normal_add = normal; }

glm::vec4 Material::getSpecularAdd() const { return normal_add; }

} // namespace gnev::tool