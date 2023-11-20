#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <unordered_map>

#include "gl/texture/TexImage.hpp"
#include "material/base/MaterialImageLoader.hpp"

namespace gnev {

class EXPORT MaterialImageLoaderStb : public base::MaterialImageLoader {
public:
    MaterialImageLoaderStb();
    virtual ~MaterialImageLoaderStb();

    std::shared_ptr<Result> upload(std::weak_ptr<base::MaterialTexStorage> tex_storage,
                                   const std::filesystem::path& path,
                                   const gl::TexImageInfo& info) override;

    struct ResultStb : public Result {
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

        ResultStb(std::future<bool> done, const base::MaterialTexRef& tex_ref)
            : Result(std::move(done), tex_ref) {}

        std::vector<Message> messages;
    };

private:
    using Buffer = std::shared_ptr<GLubyte[]>;
    std::unordered_map<std::filesystem::path, std::shared_ptr<ResultStb>> cache;

    struct StbInfo {
        int width;
        int height;
        int comp;
    };

    static std::optional<gl::TexImage> readImage(const std::filesystem::path& path,
                                                 const gl::TexImageInfo& load_info,
                                                 ResultStb& result);

    static bool validateInfo(const gl::TexImageInfo& info, ResultStb& result);
    static gl::TexImageInfo
    prepareInfo(const gl::TexImageInfo& info, const StbInfo& stb_info, ResultStb& result);

    static unsigned int getComponents(const gl::TexImageInfo& info);
    static std::size_t getBufferSize(const gl::TexImageInfo& info);

    static Buffer
    stbiLoad(const std::filesystem::path& path, StbInfo& stb_info, int req_comp = 0);
    static Buffer stbiResize(const Buffer& img,
                             const StbInfo& src_info,
                             const gl::TexImageInfo& dst_info);
};

} // namespace gnev