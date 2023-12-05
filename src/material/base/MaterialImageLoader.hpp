#pragma once

#include <filesystem>
#include <future>
#include <memory>
#include <utility>

#include "material/base/MaterialImage.hpp"
#include "material/base/MaterialTex.hpp"
#include "util/OperationResult.hpp"
#include "util/Ref.hpp"
#include "util/WeakRef.hpp"

namespace gnev::base {

class EXPORT MaterialImageLoader {
public:
    MaterialImageLoader(){};
    virtual ~MaterialImageLoader(){};

    virtual Ref<OperationResult> upload(Ref<MaterialTex> tex_ref,
                                        const std::filesystem::path& path,
                                        const gl::TexImageInfo& read_info,
                                        const gl::TexImageInfo& write_info) = 0;
};

} // namespace gnev::base