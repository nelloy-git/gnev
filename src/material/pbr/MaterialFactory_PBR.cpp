#include "material/pbr/MaterialFactory_PBR.hpp"

#include <filesystem>

namespace gnev {

MaterialFactory_PBR::MaterialFactory_PBR(Ref<MaterialStorage_PBR> storage)
    : storage(storage) {}

MaterialFactory_PBR::MaterialFactory_PBR(GLuint img_levels,
                                         GLuint img_width,
                                         GLuint img_height,
                                         GLuint capacity)
    : storage(initStorage(img_levels, img_width, img_height, capacity)) {}

MaterialFactory_PBR::~MaterialFactory_PBR(){};

Ref<MaterialStorage_PBR> MaterialFactory_PBR::getStorage() const { return storage; }

Ref<Material_PBR> MaterialFactory_PBR::createMaterial() {
    return MakeSharable<Material_PBR>(getStorage());
}

Ref<MaterialTex_PBR> MaterialFactory_PBR::createTex(MaterialTexType_PBR type) {
    return MakeSharable<MaterialTex_PBR>(storage->textures.at(toUint(type)));
}

Ref<MaterialStorage_PBR> MaterialFactory_PBR::initStorage(GLuint img_levels,
                                                          GLuint img_width,
                                                          GLuint img_height,
                                                          GLuint capacity) {
    auto data_storage = MakeSharable<MaterialDataStorage_PBR>(capacity);

    std::array<Ref<MaterialTexStorage_PBR>, 5> tex_storages = {
        MakeSharable<MaterialTexStorage_PBR>(img_levels,
                                             GL_RGBA8,
                                             img_width,
                                             img_height,
                                             capacity),
        MakeSharable<MaterialTexStorage_PBR>(img_levels,
                                             GL_RGBA8,
                                             img_width,
                                             img_height,
                                             capacity),
        MakeSharable<MaterialTexStorage_PBR>(img_levels,
                                             GL_RGBA8,
                                             img_width,
                                             img_height,
                                             capacity),
        MakeSharable<MaterialTexStorage_PBR>(img_levels,
                                             GL_RGBA8,
                                             img_width,
                                             img_height,
                                             capacity),
        MakeSharable<MaterialTexStorage_PBR>(img_levels,
                                             GL_RGBA8,
                                             img_width,
                                             img_height,
                                             capacity),
    };

    return MakeSharable<MaterialStorage_PBR>(data_storage, tex_storages);
}

} // namespace gnev