#pragma once

#include <filesystem>
#include <future>
#include <memory>

#include "gl/texture/TexImage.hpp"
#include "material/base/MaterialTexRef.hpp"
#include "material/base/MaterialTexStorage.hpp"

namespace gnev::base {
struct MaterialImageLoaderResult {
    MaterialImageLoaderResult(std::future<bool> done, const MaterialTexRef& tex_ref)
        : done(std::move(done))
        , tex_ref(tex_ref) {}
    inline virtual ~MaterialImageLoaderResult(){};

    std::future<bool> done;
    MaterialTexRef tex_ref;
};

class EXPORT MaterialImageLoader {
public:
    MaterialImageLoader(){};
    virtual ~MaterialImageLoader(){};

    virtual std::shared_ptr<MaterialImageLoaderResult>
    upload(std::weak_ptr<MaterialTexStorage> weak_tex_storage,
           const std::filesystem::path& path,
           const gl::TexImageInfo& info) = 0;
};

} // namespace gnev::base