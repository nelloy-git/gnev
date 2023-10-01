#include "gl/texture/ResizableStorage.hpp"

#include "OpenGLContext.hpp"
#include "gtest/gtest.h"

using namespace gnev::gl;

class EXPORT TestTextureResizableStorage
    : public testing::Test
    , public OpenGLContext {
public:
    std::size_t init_levels = 2;
    GLenum init_internal_format = GL_RGB8;
    std::size_t init_width = 4;
    std::size_t init_height = 4;
    std::size_t init_capacity = 4;

    texture::ResizableStorage initStorage() {
        texture::ResizableStorage storage(getCtx(),
                                          init_levels,
                                          init_internal_format,
                                          init_width,
                                          init_height,
                                          init_capacity);
        storage.generateMipmap();

        EXPECT_EQ(init_levels, storage.getLevels());
        EXPECT_EQ(init_internal_format, storage.getLevelInternalFormat(0));
        EXPECT_EQ(init_width, storage.getLevelWidth(0));
        EXPECT_EQ(init_height, storage.getLevelHeight(0));
        EXPECT_EQ(init_width / 2, storage.getLevelWidth(1));
        EXPECT_EQ(init_height / 2, storage.getLevelHeight(1));
        EXPECT_EQ(init_capacity, storage.getCapacity());

        return storage;
    };
};

TEST_F(TestTextureResizableStorage, setElement_getElement) {
    auto storage = initStorage();

    std::shared_ptr<void> img_data =
        std::make_shared<char[]>(init_width * init_height * 3);

    {
        unsigned char* raw_img = static_cast<unsigned char*>(img_data.get());
        for (std::size_t i = 0; i < init_width * init_height * 3; ++i) {
            raw_img[i] = i;
        }
    }

    for (int i = 0; i < init_capacity; ++i) {
        texture::Image img{.level = 0,
                           .x = 0,
                           .y = 0,
                           .width = init_width,
                           .height = init_height,
                           .format = GL_RGB,
                           .type = GL_UNSIGNED_BYTE,
                           .data = img_data};
        storage.setElement(i, img);
    }

    for (int i = 0; i < init_capacity; ++i) {
        auto img = storage.getElement(i, 0, GL_RGB, GL_UNSIGNED_BYTE);

        EXPECT_EQ(img.level, 0);
        EXPECT_EQ(img.x, 0);
        EXPECT_EQ(img.y, 0);
        EXPECT_EQ(img.width, init_width);
        EXPECT_EQ(img.height, init_height);

        unsigned char* raw_img = static_cast<unsigned char*>(img.data.get());
        for (std::size_t i = 0; i < init_width * init_height * 3; ++i) {
            EXPECT_EQ(i, raw_img[i]);
        }
    }
}

TEST_F(TestTextureResizableStorage, copyRange) {
    auto storage = initStorage();

    std::shared_ptr<void> img_data =
        std::make_shared<char[]>(init_width * init_height * 3);

    {
        unsigned char* raw_img = static_cast<unsigned char*>(img_data.get());
        for (std::size_t i = 0; i < init_width * init_height * 3; ++i) {
            raw_img[i] = i;
        }
    }

    texture::Image img{.level = 0,
                       .x = 0,
                       .y = 0,
                       .width = init_width,
                       .height = init_height,
                       .format = GL_RGB,
                       .type = GL_UNSIGNED_BYTE,
                       .data = img_data};
    storage.setElement(0, img);
    storage.copyRange(0, 0, 1);
    storage.copyRange(0, 1, 1);
    storage.copyRange(0, 2, 2);
    

    for (int i = 0; i < init_capacity; ++i) {
        auto img = storage.getElement(i, 0, GL_RGB, GL_UNSIGNED_BYTE);

        EXPECT_EQ(img.level, 0);
        EXPECT_EQ(img.x, 0);
        EXPECT_EQ(img.y, 0);
        EXPECT_EQ(img.width, init_width);
        EXPECT_EQ(img.height, init_height);

        unsigned char* raw_img = static_cast<unsigned char*>(img.data.get());
        for (std::size_t i = 0; i < init_width * init_height * 3; ++i) {
            EXPECT_EQ(i, raw_img[i]);
        }
    }
}