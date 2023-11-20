#include "gl/texture/TexStorage.hpp"

#include "OpenGLContext.hpp"
#include "gl/texture/TexImage.hpp"
#include "gtest/gtest.h"

using namespace gnev::gl;

class EXPORT TestTexStorage
    : public testing::Test
    , public OpenGLContext {
public:
    std::size_t init_levels = 2;
    GLenum init_internal_format = GL_RGB8;
    std::size_t init_width = 8;
    std::size_t init_height = 8;
    std::size_t init_capacity = 4;
    static constexpr auto init_value = [](GLuint level, GLuint pos) { return 0; };

    TexStorage initStorage(std::function<GLubyte(GLuint level, GLuint pos)> initial =
                               init_value) const {
        TexStorage storage(init_levels,
                           init_width,
                           init_height,
                           init_capacity,
                           init_internal_format);

        EXPECT_EQ(init_levels, storage.getLevels());
        EXPECT_EQ(init_capacity, storage.getCapacity());

        GLuint img_bytes = init_width * init_height * 3;
        auto buffer = std::make_unique<GLubyte[]>(img_bytes);

        for (int l = 0; l < init_levels; ++l) {
            auto level_width = init_width / (1 << l);
            auto level_height = init_height / (1 << l);
            auto level_bytes = level_width * level_height * 3;

            EXPECT_EQ(level_width, storage.getLevelWidth(l));
            EXPECT_EQ(level_height, storage.getLevelHeight(l));
            EXPECT_EQ(level_bytes % 16, 0);

            for (int i = 0; i < img_bytes; ++i) {
                buffer[i] = initial(l, i);
            }

            for (int i = 0; i < init_capacity; ++i) {
                EXPECT_EQ(level_width, storage[i].getLevelWidth(l));
                EXPECT_EQ(level_height, storage[i].getLevelHeight(l));

                getCtx().glTextureSubImage3D(storage.handle(),
                                             l,
                                             0,
                                             0,
                                             i,
                                             level_width,
                                             level_height,
                                             1,
                                             GL_RGB,
                                             GL_UNSIGNED_BYTE,
                                             buffer.get());

                auto tmp = std::make_unique<GLubyte[]>(level_bytes);
                getCtx().glGetTextureSubImage(storage.handle(),
                                              l,
                                              0,
                                              0,
                                              i,
                                              level_width,
                                              level_height,
                                              1,
                                              GL_RGB,
                                              GL_UNSIGNED_BYTE,
                                              level_bytes,
                                              tmp.get());

                for (GLuint j = 0; j < level_bytes; ++j) {
                    EXPECT_EQ(buffer[j], tmp[j]);
                }
            }
        }

        return storage;
    };

    TexImageInfo initImageInfo(GLuint level = 0) const {
        TexImageInfo info;
        info.level = level;
        info.width = init_width / (1 << level);
        info.height = init_height / (1 << level);
        info.format = GL_RGB;
        info.type = GL_UNSIGNED_BYTE;
        return info;
    }
};

TEST_F(TestTexStorage, rawSet) {
    auto storage = initStorage([](GLuint level, GLuint pos) { return 97 + pos; });

    GLuint img_bytes = init_width * init_height * 3;
    auto buffer = std::make_unique<GLubyte[]>(img_bytes);
    for (int i = 0; i < img_bytes; ++i) {
        buffer[i] = i;
    }

    for (int i = 0; i < init_capacity; ++i) {
        getCtx().glTextureSubImage3D(storage.handle(),
                                     0,
                                     0,
                                     0,
                                     i,
                                     init_width,
                                     init_height,
                                     1,
                                     GL_RGB,
                                     GL_UNSIGNED_BYTE,
                                     buffer.get());
    }

    for (int i = 0; i < init_capacity; ++i) {
        auto tmp = std::make_unique<GLubyte[]>(img_bytes);
        getCtx().glGetTextureSubImage(storage.handle(),
                                      0,
                                      0,
                                      0,
                                      i,
                                      init_width,
                                      init_height,
                                      1,
                                      GL_RGB,
                                      GL_UNSIGNED_BYTE,
                                      img_bytes,
                                      tmp.get());

        for (GLuint j = 0; j < img_bytes; ++j) {
            EXPECT_EQ(j, GLuint(tmp[j]));
        }
    }
}

TEST_F(TestTexStorage, setImage) {
    auto storage = initStorage();

    std::vector<std::vector<TexImage>> images;
    for (GLuint i = 0; i < init_capacity; ++i) {
        images.push_back({});

        for (GLuint l = 0; l < init_levels; ++l) {
            TexImageInfo info = initImageInfo(l);
            auto level_bytes = info.width * info.height * 3;

            images[i].push_back({info, level_bytes});
            for (GLuint j = 0; j < level_bytes; ++j) {
                images[i][l].data.data<GLubyte>()[j] = j;
            }
        }
    }

    for (int i = 0; i < init_capacity; ++i) {
        for (GLuint l = 0; l < init_levels; ++l) {
            storage[i].setImage(images[i][l]);
        }
    }

    for (int l = 0; l < init_levels; ++l) {
        auto level_width = init_width / (1 << l);
        auto level_height = init_height / (1 << l);
        auto level_bytes = level_width * level_height * 3;

        for (int i = 0; i < init_capacity; ++i) {
            auto tmp = std::make_unique<GLubyte[]>(level_bytes);
            getCtx().glGetTextureSubImage(storage.handle(),
                                          l,
                                          0,
                                          0,
                                          i,
                                          level_width,
                                          level_height,
                                          1,
                                          GL_RGB,
                                          GL_UNSIGNED_BYTE,
                                          level_bytes,
                                          tmp.get());

            for (GLuint j = 0; j < level_bytes; ++j) {
                EXPECT_EQ(images[i][l].data.data<GLubyte>()[j], tmp[j]);
            }
        }
    }
}

TEST_F(TestTexStorage, getImage) {
    auto storage = initStorage();

    std::vector<std::vector<TexImage>> images;
    for (GLuint i = 0; i < init_capacity; ++i) {
        images.push_back({});

        for (GLuint l = 0; l < init_levels; ++l) {
            TexImageInfo info = initImageInfo(l);
            auto level_bytes = info.width * info.height * 3;

            images[i].push_back({info, level_bytes});
            for (GLuint j = 0; j < level_bytes; ++j) {
                images[i][l].data.data<GLubyte>()[j] = j;
            }
        }
    }

    for (int i = 0; i < init_capacity; ++i) {
        for (GLuint l = 0; l < init_levels; ++l) {
            getCtx().glTextureSubImage3D(storage.handle(),
                                         l,
                                         0,
                                         0,
                                         i,
                                         images[i][l].info.width,
                                         images[i][l].info.height,
                                         1,
                                         GL_RGB,
                                         GL_UNSIGNED_BYTE,
                                         images[i][l].data.data());
        }
    }

    for (int l = 0; l < init_levels; ++l) {
        auto level_width = init_width / (1 << l);
        auto level_height = init_height / (1 << l);
        auto level_bytes = level_width * level_height * 3;

        for (int i = 0; i < init_capacity; ++i) {
            auto res = storage[i].getImage(images[i][l].info);
            for (GLuint j = 0; j < level_bytes; ++j) {
                EXPECT_EQ(images[i][l].data.data<GLubyte>()[j],
                          res.data.data<GLubyte>()[j]);
            }
        }
    }
}
