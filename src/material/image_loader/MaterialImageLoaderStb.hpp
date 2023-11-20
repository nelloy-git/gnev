#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <unordered_map>

#include "gl/texture/TexImage.hpp"
#include "material/base/MaterialImageLoader.hpp"

namespace gnev {
struct MaterialImageLoaderStbResult : public base::MaterialImageLoaderResult {
    enum class Message {
        Done,
        Failed,
        UnsupportedLevel,
        UnsupportedX,
        UnsupportedY,
        UnsupportedWidth,
        UnsupportedHeight,
        UnsupportedFormat,
        UnsupportedType,
        UnsupportedInfo,
        FileDoNotExist,
        OriginalImageHasDifferentFormat,
        ImageResized,
        AutoFormat,
        AutoWidth,
        AutoHeight
    };

    MaterialImageLoaderStbResult(std::future<bool> done,
                                 const base::MaterialTexRef& tex_ref)
        : MaterialImageLoaderResult(std::move(done), tex_ref) {}

    ~MaterialImageLoaderStbResult(){};

    std::vector<Message> messages;
};

class EXPORT MaterialImageLoaderStb : public base::MaterialImageLoader {
public:
    MaterialImageLoaderStb();
    virtual ~MaterialImageLoaderStb();

    std::shared_ptr<base::MaterialImageLoaderResult>
    upload(std::weak_ptr<base::MaterialTexStorage> tex_storage,
           const std::filesystem::path& path,
           const gl::TexImageInfo& info) override;

private:
    using Buffer = std::shared_ptr<GLubyte[]>;
    std::unordered_map<std::filesystem::path,
                       std::shared_ptr<MaterialImageLoaderStbResult>>
        cache;

    struct StbInfo {
        int width;
        int height;
        int comp;
    };

    static std::optional<gl::TexImage> readImage(const std::filesystem::path& path,
                                                 const gl::TexImageInfo& load_info,
                                                 MaterialImageLoaderStbResult& result);

    static bool
    validateInfo(const gl::TexImageInfo& info, MaterialImageLoaderStbResult& result);
    static gl::TexImageInfo prepareInfo(const gl::TexImageInfo& info,
                                        const StbInfo& stb_info,
                                        MaterialImageLoaderStbResult& result);

    static unsigned int getComponents(const gl::TexImageInfo& info);
    static std::size_t getBufferSize(const gl::TexImageInfo& info);

    static Buffer
    stbiLoad(const std::filesystem::path& path, StbInfo& stb_info, int req_comp = 0);
    static Buffer stbiResize(const Buffer& img,
                             const StbInfo& src_info,
                             const gl::TexImageInfo& dst_info,
                             MaterialImageLoaderStbResult& result);
};

} // namespace gnev