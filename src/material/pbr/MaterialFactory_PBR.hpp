#pragma once

#include "material/base/MaterialStorage.hpp"
#include "material/base/MaterialTex.hpp"
#include "material/base/MaterialImageLoader.hpp"
#include "material/pbr/MaterialGL_PBR.hpp"
#include "material/pbr/Material_PBR.hpp"
#include "util/Ref.hpp"

namespace std::filesystem {
class path;
};

namespace gnev {

using MaterialGL_PBR = MaterialGL_PBR;
using Material_PBR = Material_PBR;

using MaterialStorage_PBR = base::MaterialStorage<MaterialGL_PBR>;
using MaterialDataStorage_PBR = base::MaterialDataStorage<MaterialGL_PBR>;
using MaterialTexStorage_PBR = base::MaterialTexStorage;

using MaterialData_PBR = base::MaterialData<MaterialGL_PBR>;
using MaterialTex_PBR = base::MaterialTex;

class EXPORT MaterialFactory_PBR {
public:
    MaterialFactory_PBR(Ref<MaterialStorage_PBR>);
    MaterialFactory_PBR(GLuint img_levels,
                        GLuint img_width,
                        GLuint img_height,
                        GLuint capacity);
    virtual ~MaterialFactory_PBR();

    Ref<MaterialStorage_PBR> getStorage() const;

    Ref<Material_PBR> createMaterial();
    Ref<MaterialTex_PBR> createTex(MaterialTexType_PBR type);

private:
    Ref<MaterialStorage_PBR> storage;

    static Ref<MaterialStorage_PBR>
    initStorage(GLuint img_levels, GLuint img_width, GLuint img_height, GLuint capacity);
};

} // namespace gnev