#pragma once

#include <optional>

#include "material/base/MaterialTexStorage.hpp"
#include "util/Export.hpp"
#include "util/Ref.hpp"
#include "util/WeakRef.hpp"

namespace gnev::base {

class EXPORT MaterialTex {
public:
    MaterialTex(WeakRef<MaterialTexStorage> weak_storage);
    virtual ~MaterialTex();

    WeakRef<MaterialTexStorage> getWeakStorage() const;
    Ref<GLuint> getIndex() const;
    GLuint getTexType() const;
    bool setData(const gl::TexImage& src);
    bool getData(gl::TexImage& src) const;

private:
    WeakRef<MaterialTexStorage> weak_storage;
    Ref<GLuint> index_keeper;
    GLuint tex_type;

    static Ref<GLuint> initIndexKeeper(const WeakRef<MaterialTexStorage>& weak_storage);
};

} // namespace gnev::base