#pragma once

#include <filesystem>
#include <future>
#include <memory>

#include "gl/texture/TexImage.hpp"
#include "material/base/Defines.hpp"

namespace gnev::base {

template <IsTriviallyCopyable T>
class MaterialStorage;

struct EXPORT MaterialImageUploadResult {
    MaterialImageUploadResult(std::promise<void>& promise)
        : finished(promise.get_future()) {}

    std::future<void> finished;
    std::optional<GLuint> index;
};

class EXPORT MaterialImageLoader {
    template <IsTriviallyCopyable T>
    friend class MaterialStorage;

public:
    using UploadResult = std::shared_ptr<const MaterialImageUploadResult>;

    MaterialImageLoader(const std::shared_ptr<MaterialTexStorage>& tex_storage)
        : tex_storage(tex_storage){};
    virtual ~MaterialImageLoader(){};

    virtual UploadResult upload(const std::filesystem::path& path,
                                const gl::TexImageInfo& info) = 0;

protected:
    std::shared_ptr<MaterialTexStorage> tex_storage;
};

} // namespace gnev::base