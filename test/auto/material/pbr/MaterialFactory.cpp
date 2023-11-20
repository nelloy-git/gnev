#include "OpenGLContext.hpp"
#include "gtest/gtest.h"
#include "material/base/MaterialImageLoader.hpp"
#include "material/pbr/MaterialFactory_PBR.hpp"

using namespace gnev;

class EXPORT TestMaterialFactory
    : public testing::Test
    , public OpenGLContext {
public:
    static constexpr GLuint capacity = 10;
    static constexpr GLbitfield storage_flags = GL_DYNAMIC_STORAGE_BIT;
    static constexpr GLuint levels = 2;
    static constexpr GLuint width = 4;
    static constexpr GLuint height = 4;

    class Loader : public base::MaterialImageLoader {
    public:
        gl::TexImage to_write;

        std::shared_ptr<base::MaterialImageLoaderResult>
        upload(std::weak_ptr<base::MaterialTexStorage> weak_tex_storage,
               const std::filesystem::path& path,
               const gl::TexImageInfo& info) override {

            std::promise<bool> done;
            base::MaterialTexRef tex_ref(weak_tex_storage);
            auto result =
                std::make_shared<base::MaterialImageLoaderResult>(done.get_future(),
                                                                  tex_ref);
            weak_tex_storage.lock()->at(*tex_ref.getIndex()).setImage(to_write);
            done.set_value(true);
            return result;
        }
    };

    bool upload(std::weak_ptr<base::MaterialTexStorage> weak_tex_storage,
                const gl::TexImage& to_write) {
        Loader loader;
        loader.to_write = to_write;
        auto result = loader.upload(weak_tex_storage, "", to_write.info);
        return result->done.get();
    }

    MaterialFactory_PBR initFactory() {
        auto data_storage =
            std::make_shared<gnev::MaterialDataStorage_PBR>(GL_DYNAMIC_STORAGE_BIT,
                                                            capacity,
                                                            std::nullopt);
        // clang-format off
        std::array<const std::shared_ptr<gnev::MaterialTexStorage_PBR>, 5> tex_storages = {
            std::make_shared<gnev::MaterialTexStorage_PBR>(capacity, 1, width, height, GL_RGBA8, std::nullopt),
            std::make_shared<gnev::MaterialTexStorage_PBR>(capacity, 1, width, height, GL_RGBA8, std::nullopt),
            std::make_shared<gnev::MaterialTexStorage_PBR>(capacity, 1, width, height, GL_RGBA8, std::nullopt),
            std::make_shared<gnev::MaterialTexStorage_PBR>(capacity, 1, width, height, GL_RGBA8, std::nullopt),
            std::make_shared<gnev::MaterialTexStorage_PBR>(capacity, 1, width, height, GL_RGBA8, std::nullopt),
        };
        // clang-format on
        auto storage =
            std::make_shared<gnev::MaterialStorage_PBR>(data_storage, tex_storages);
        return gnev::MaterialFactory_PBR(storage);
    }
};

TEST_F(TestMaterialFactory, createMaterial) {
    auto factory = initFactory();
    auto material = factory.create();
    // material.
}