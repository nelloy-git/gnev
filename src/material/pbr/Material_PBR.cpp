#include "material/pbr/Material_PBR.hpp"

#include <cstddef>
#include <memory>
#include <stdexcept>

#include "material/base/Material.hpp"
#include "material/base/MaterialStorage.hpp"
#include "material/pbr/MaterialGL_PBR.hpp"

namespace gnev {

using Base = base::Material<MaterialGL_PBR>;

Material_PBR::Material_PBR(const std::weak_ptr<MaterialStorage_PBR>& weak_storage)
    : Base(weak_storage) {}

Material_PBR::~Material_PBR() {}

std::weak_ptr<MaterialStorage_PBR> Material_PBR::getWeakStorage() const {
    return lockStorage();
}

std::shared_ptr<MaterialStorage_PBR> Material_PBR::lockStorage() const {
    return std::dynamic_pointer_cast<MaterialStorage_PBR>(Base::lockStorage());
}

std::optional<base::MaterialTexRef>
Material_PBR::getTexRef(MaterialTexType_PBR type) const {
    return Base::getTexRef(toUint(type));
}

void Material_PBR::setTexRef(MaterialTexType_PBR type,
                             std::optional<base::MaterialTexRef> tex_ref) {
    Base::setTexRef(toUint(type), tex_ref);
}

std::shared_ptr<base::MaterialImageLoader::Result>
Material_PBR::loadTex(MaterialTexType_PBR type,
                      base::MaterialImageLoader& loader,
                      const std::filesystem::path& path,
                      const gl::TexImageInfo& info) {
    return Base::loadTex(toUint(type), loader, path, info);
}

void Material_PBR::setTexOffset(MaterialTexType_PBR type, const glm::vec4& value) {
    auto [storage, iter] = getDataRef().lockIter();
    iter.copyFrom(&value,
                  MaterialGL_PBR::OffsetOfTexOffset(type),
                  MaterialGL_PBR::SizeOfTexOffsetElem());
}

glm::vec4 Material_PBR::getTexOffset(MaterialTexType_PBR type) const {
    auto [storage, iter] = getDataRef().lockIter();
    glm::vec4 result;
    iter.copyTo(&result,
                MaterialGL_PBR::OffsetOfTexOffset(type),
                MaterialGL_PBR::SizeOfTexOffsetElem());
    return result;
}

void Material_PBR::setTexMultiplier(MaterialTexType_PBR type, const glm::vec4& value) {
    auto [storage, iter] = getDataRef().lockIter();
    iter.copyFrom(&value,
                  MaterialGL_PBR::OffsetOfTexMultiplier(type),
                  MaterialGL_PBR::SizeOfTexMultiplierElem());
}

glm::vec4 Material_PBR::getTexMultiplier(MaterialTexType_PBR type) const {
    auto [storage, iter] = getDataRef().lockIter();

    glm::vec4 result;
    iter.copyTo(&result,
                MaterialGL_PBR::OffsetOfTexMultiplier(type),
                MaterialGL_PBR::SizeOfTexMultiplierElem());
    return result;
}

} // namespace gnev