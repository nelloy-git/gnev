#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <unordered_map>

#include "material/base/MaterialImage.hpp"
#include "material/base/MaterialImageLoader.hpp"
#include "material/image_loader/MaterialImageLoaderStbiResult.hpp"
#include "util/Ref.hpp"

namespace gnev {

class EXPORT MaterialImageLoaderStbi : public base::MaterialImageLoader {
public:
    MaterialImageLoaderStbi();
    virtual ~MaterialImageLoaderStbi();

    Ref<OperationResult>
    upload(Ref<base::MaterialTex> tex_ref,
           const std::filesystem::path& path,
           const gl::TexImageInfo& read_info,
           const gl::TexImageInfo& write_info) override;

private:
    using Buffer = std::shared_ptr<GLubyte[]>;
    std::unordered_map<std::wstring, Ref<MaterialImageLoaderStbiResult>> cache;

    struct StbInfo {
        int width;
        int height;
        int comp;
    };

    static std::optional<gl::TexImage> readImage(const std::filesystem::path& path,
                                                 const gl::TexImageInfo& read_info,
                                                 const gl::TexImageInfo& write_info,
                                                 MaterialImageLoaderStbiResult& result);

    static bool validateInfos(const gl::TexImageInfo& read_info,
                              const gl::TexImageInfo& write_info,
                              MaterialImageLoaderStbiResult& result);

    static unsigned int getComponents(const gl::TexImageInfo& info);
    static std::size_t getBufferSize(const gl::TexImageInfo& info);

    static Buffer
    stbiLoad(const std::filesystem::path& path, StbInfo& stb_info, int req_comp = 0);
    static Buffer stbiResize(const Buffer& img,
                             const StbInfo& src_info,
                             const gl::TexImageInfo& dst_info,
                             MaterialImageLoaderStbiResult& result);
};

} // namespace gnev