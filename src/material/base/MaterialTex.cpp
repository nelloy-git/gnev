#include "material/base/MaterialTex.hpp"

#include <stdexcept>

namespace gnev::base {

MaterialTex::MaterialTex(WeakRef<MaterialTexStorage> weak_storage)
    : weak_storage(weak_storage)
    , index_keeper(initIndexKeeper(weak_storage)) {}

MaterialTex::~MaterialTex() {}

WeakRef<MaterialTexStorage> MaterialTex::getWeakStorage() const { return weak_storage; }

Ref<GLuint> MaterialTex::getIndex() const { return index_keeper; }

GLuint MaterialTex::getTexType() const { return tex_type; }

bool MaterialTex::setData(const gl::TexImage& src) {
    auto storage_opt = weak_storage.lock();
    if (not storage_opt.has_value()) {
        return false;
    }
    return storage_opt.value()->setData(index_keeper, src);
}

bool MaterialTex::getData(gl::TexImage& src) const {
    auto storage_opt = weak_storage.lock();
    if (not storage_opt.has_value()) {
        return false;
    }
    return storage_opt.value()->getData(index_keeper, src);
}

Ref<GLuint>
MaterialTex::initIndexKeeper(const WeakRef<MaterialTexStorage>& weak_storage) {
    auto storage_opt = weak_storage.lock();
    if (not storage_opt.has_value()) {
        throw std::runtime_error("");
    }

    GLuint index = storage_opt.value()->initIndex();

    auto del = [weak_storage](GLuint* p_index) {
        auto storage_opt = weak_storage.lock();
        if (storage_opt.has_value()) {
            storage_opt.value()->freeIndex(*p_index);
        }
        delete p_index;
    };
    auto shared_index = std::shared_ptr<GLuint>(new GLuint(index), del);
    return Ref<GLuint>(shared_index);
}

} // namespace gnev::base