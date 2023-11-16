#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <unordered_map>

#include "gl/texture/TexImage.hpp"
#include "material/base/MaterialImageLoader.hpp"

namespace gnev {

struct MaterialImageUploadResultStb : public base::MaterialImageUploadResult {
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

    std::vector<Message> messages;
};

class EXPORT MaterialImageLoaderStb : public base::MaterialImageLoader {
public:
    MaterialImageLoaderStb(const std::shared_ptr<base::MaterialTexStorage>& tex_storage);
    virtual ~MaterialImageLoaderStb();

    UploadResult upload(const std::filesystem::path& path,
                        const gl::TexImageInfo& info) override;

private:
    using Buffer = std::shared_ptr<GLubyte[]>;

    std::unordered_map<std::filesystem::path, std::shared_ptr<MaterialImageUploadResultStb>> cache;

    struct StbInfo {
        int width;
        int height;
        int comp;
    };

    static std::optional<gl::TexImage> readImage(const std::filesystem::path& path,
                                                 const gl::TexImageInfo& load_info,
                                                 MaterialImageUploadResultStb& result);

    static bool validateInfo(const gl::TexImageInfo& info,
                             MaterialImageUploadResultStb& result);
    static gl::TexImageInfo prepareInfo(const gl::TexImageInfo& info,
                                        const StbInfo& stb_info,
                                        MaterialImageUploadResultStb& result);

    static unsigned int getComponents(const gl::TexImageInfo& info);
    static std::size_t getBufferSize(const gl::TexImageInfo& info);

    static Buffer stbiLoad(const std::filesystem::path& path,
                           StbInfo& stb_info,
                           int req_comp = 0);
    static Buffer stbiResize(const Buffer& img,
                             const StbInfo& src_info,
                             const gl::TexImageInfo& dst_info);
};

} // namespace gnev