#pragma once

#include <filesystem>
#include <future>
#include <memory>
#include <utility>

#include "gl/texture/TexImage.hpp"
#include "material/base/MaterialTex.hpp"
#include "util/StrongRef.hpp"
#include "util/WeakRef.hpp"

namespace gnev::base {

struct MaterialImageLoaderResult {
    MaterialImageLoaderResult(std::shared_future<bool>&& done,
                              WeakRef<MaterialTex> tex_ref)
        : done(std::forward<decltype(done)>(done)){}

    virtual ~MaterialImageLoaderResult(){};

    std::shared_future<bool> done;
};

class EXPORT MaterialImageLoader {
public:
    MaterialImageLoader(){};
    virtual ~MaterialImageLoader(){};

    virtual StrongRef<MaterialImageLoaderResult>
    upload(StrongRef<MaterialTex> tex_ref,
           const std::filesystem::path& path,
           const gl::TexImageInfo& read_info,
           const gl::TexImageInfo& write_info) = 0;
};

} // namespace gnev::base