#pragma once

#include <filesystem>
#include <future>
#include <memory>
#include <utility>

#include "image/Image.hpp"
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
                                        const Image& image) = 0;
};

} // namespace gnev::base