#pragma once

#include <filesystem>
#include <future>
#include <memory>

#include "material/base/MaterialStorage.hpp"

namespace gnev::base {

template <IsMaterialData T>
class MaterialStorage;

class EXPORT MaterialImageLoader {
    template <IsMaterialData T>
    friend class MaterialStorage;

public:
    struct Result {
        Result(std::future<bool>&& read_done, std::future<bool>&& upload_done)
            : read_done(std::move(read_done))
            , upload_done(std::move(upload_done)){};

        std::future<bool> read_done;
        std::future<bool> upload_done;
    };

    MaterialImageLoader(){};
    virtual ~MaterialImageLoader(){};

    virtual std::shared_ptr<Result> upload(std::shared_ptr<MaterialTexRefStorage>
                                               tex_storage,
                                           GLuint tex_index,
                                           const std::filesystem::path& path,
                                           const gl::TexImageInfo& info) = 0;
};

} // namespace gnev::base