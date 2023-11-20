#pragma once

#include <filesystem>
#include <future>
#include <memory>

#include "gl/texture/TexImage.hpp"
#include "material/base/MaterialTexRef.hpp"

namespace gnev::base {

class EXPORT MaterialImageLoader {
public:
    struct Result {
        Result(std::future<bool> done, const MaterialTexRef& tex_ref)
            : done(std::move(done))
            , tex_ref(tex_ref) {}

        std::future<bool> done;
        MaterialTexRef tex_ref;
    };

    MaterialImageLoader(){};
    virtual ~MaterialImageLoader(){};

    virtual std::shared_ptr<Result>
    upload(std::weak_ptr<MaterialTexStorage> weak_tex_storage,
           const std::filesystem::path& path,
           const gl::TexImageInfo& info) = 0;
};

} // namespace gnev::base