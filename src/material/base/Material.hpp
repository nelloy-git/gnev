#pragma once

#include <memory>

#include "material/base/Defines.hpp"

namespace gnev::base {

template <IsTriviallyCopyable T, MaterialIndex TexN>
class MaterialStorage;

template <IsTriviallyCopyable T, MaterialIndex TexN>
class EXPORT Material {
public:
    Material(const std::shared_ptr<MaterialStorage<T, TexN>>& storage,
             MaterialIndex index)
        : storage(storage)
        , index(index){};

    virtual ~Material();

    std::shared_ptr<MaterialStorage<T, TexN>> getStorage() const;

    virtual MaterialIndex getDataIndex() const;
    virtual T getData() const;
    virtual void setData(const T&);


private:
    std::weak_ptr<MaterialStorage<T, TexN>> storage;
    MaterialIndex index;
};

} // namespace gnev::base