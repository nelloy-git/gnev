#include "gl/texture/ImmutableStorage.hpp"

#include "OpenGLContext.hpp"
#include "gl/texture/Image.hpp"
#include "gtest/gtest.h"

using namespace gnev::gl;

class EXPORT TestTextureImmutableStorage
    : public testing::Test
    , public OpenGLContext {
public:
    std::size_t init_levels = 2;
    GLenum init_internal_format = GL_RGB8;
    std::size_t init_width = 4;
    std::size_t init_height = 4;
    std::size_t init_capacity = 4;

    texture::ImmutableStorage initStorage() {
        texture::ImmutableStorage storage(init_levels,
                                          init_width,
                                          init_height,
                                          init_capacity,
                                          init_internal_format);
        // storage.generateMipmap();

        EXPECT_EQ(init_levels, storage.getLevels());
        // EXPECT_EQ(init_internal_format, storage.getLevelInternalFormat(0));
        // EXPECT_EQ(init_width, storage.getLevelWidth(0));
        // EXPECT_EQ(init_height, storage.getLevelHeight(0));
        // EXPECT_EQ(init_width / 2, storage.getLevelWidth(1));
        // EXPECT_EQ(init_height / 2, storage.getLevelHeight(1));
        EXPECT_EQ(init_capacity, storage.getCapacity());

        return storage;
    };
};

TEST_F(TestTextureImmutableStorage, setImage_getImage) {
    auto storage = initStorage();

    std::size_t img_bytes = init_width * init_height * 3;
    std::shared_ptr<GLubyte[]> img_buffer(new GLubyte[img_bytes]);
    texture::ImageData img_data(img_bytes, img_buffer);

    unsigned char* raw_img = static_cast<unsigned char*>(img_buffer.get());
    for (std::size_t i = 0; i < init_width * init_height * 3; ++i) {
        raw_img[i] = 2 * i;
    }

    texture::ImageInfo info;
    info.format = GL_RGB;
    info.type = GL_UNSIGNED_BYTE;

    for (int i = 0; i < init_capacity; ++i) {
        texture::Image img(info, img_data);
        storage[i].setImage(img);
    }

    for (int i = 0; i < init_capacity; ++i) {
        texture::ImageInfo img_info(info);
        auto img = storage[i].getImage(img_info);

        EXPECT_EQ(img_info.level, 0);
        EXPECT_EQ(img_info.x, 0);
        EXPECT_EQ(img_info.y, 0);
        EXPECT_EQ(img_info.width, init_width);
        EXPECT_EQ(img_info.height, init_height);

        unsigned char* raw_img = static_cast<unsigned char*>(img.data.data());
        for (std::size_t i = 0; i < init_width * init_height * 3; ++i) {
            EXPECT_EQ(2 * i, raw_img[i]);
        }
    }
}

TEST_F(TestTextureImmutableStorage, setImage_getImagePreallocated) {
    auto storage = initStorage();

    std::size_t img_bytes = init_width * init_height * 3;
    std::shared_ptr<GLubyte[]> img_buffer(new GLubyte[img_bytes]);
    texture::ImageData img_data(img_bytes, img_buffer);

    unsigned char* raw_img = static_cast<unsigned char*>(img_buffer.get());
    for (std::size_t i = 0; i < init_width * init_height * 3; ++i) {
        raw_img[i] = 2 * i;
    }

    texture::ImageInfo info;
    info.format = GL_RGB;
    info.type = GL_UNSIGNED_BYTE;

    for (int i = 0; i < init_capacity; ++i) {
        texture::Image img(info, img_data);
        storage[i].setImage(img);
    }

    texture::Image img(info, img_data);
    for (int i = 0; i < init_capacity; ++i) {
        storage[i].getImage(img);

        EXPECT_EQ(img.info.level, 0);
        EXPECT_EQ(img.info.x, 0);
        EXPECT_EQ(img.info.y, 0);
        EXPECT_EQ(img.info.width, init_width);
        EXPECT_EQ(img.info.height, init_height);

        unsigned char* raw_img = static_cast<unsigned char*>(img.data.data());
        for (std::size_t i = 0; i < init_width * init_height * 3; ++i) {
            EXPECT_EQ(2 * i, raw_img[i]);
        }
    }
}