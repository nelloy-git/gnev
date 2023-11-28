#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <unordered_map>

#include "gl/texture/TexImage.hpp"
#include "material/base/MaterialImageLoader.hpp"
#include "util/StrongRef.hpp"

namespace gnev {
struct MaterialImageLoaderStbiResult : public base::MaterialImageLoaderResult {
    enum class Message {
        Done,
        Failed,

        UnsupportedReadLevel,
        UnsupportedReadX,
        UnsupportedReadY,
        UnsupportedReadWidth,
        UnsupportedReadHeight,
        UnsupportedReadFormat,
        UnsupportedReadType,
        UnsupportedReadInfo,

        UnsupportedWriteLevel,
        UnsupportedWriteX,
        UnsupportedWriteY,
        UnsupportedWriteWidth,
        UnsupportedWriteHeight,
        UnsupportedWriteFormat,
        UnsupportedWriteType,
        UnsupportedWriteInfo,

        ReleasedStorage,
        FileDoNotExist,
        OriginalImageHasDifferentNumberOfComponents,
        ImageResized,
        AutoWidth,
        AutoHeight
    };

    MaterialImageLoaderStbiResult(std::shared_future<bool>&& done,
                                  WeakRef<base::MaterialTex> tex_ref)
        : MaterialImageLoaderResult(std::forward<decltype(done)>(done), tex_ref) {}

    ~MaterialImageLoaderStbiResult(){};

    std::vector<Message> messages;
};

class EXPORT MaterialImageLoaderStbi : public base::MaterialImageLoader {
public:
    MaterialImageLoaderStbi();
    virtual ~MaterialImageLoaderStbi();

    StrongRef<base::MaterialImageLoaderResult>
    upload(StrongRef<base::MaterialTex> tex_ref,
           const std::filesystem::path& path,
           const gl::TexImageInfo& read_info,
           const gl::TexImageInfo& write_info) override;

private:
    using Buffer = std::shared_ptr<GLubyte[]>;
    std::unordered_map<std::filesystem::path, StrongRef<MaterialImageLoaderStbiResult>>
        cache;

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