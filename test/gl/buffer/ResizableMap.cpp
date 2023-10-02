#include "gl/buffer/ResizableMap.hpp"

#include <array>

#include "OpenGLContext.hpp"
#include "gtest/gtest.h"

using namespace gnev::gl;

class EXPORT TestBufferResizableMap
    : public testing::Test
    , public OpenGLContext {
public:
    GLenum usage = GL_STATIC_DRAW;
    std::size_t capacity = 10;
    int init = 0;

    buffer::ResizableMap<int, std::array<char, 5>> initBuffer() {
        buffer::ResizableMap<int, std::array<char, 5>> buffer(getCtx(), usage, capacity);

        EXPECT_EQ(usage, buffer.getUsage());
        EXPECT_EQ(capacity, buffer.getCapacity());

        return std::move(buffer);
    };
};

TEST_F(TestBufferResizableMap, ctor) {
    auto usage = GL_DYNAMIC_COPY;

    std::initializer_list<std::pair<int, int>> initial = {{1, 3}, {2, 1}, {3, 2}};

    buffer::ResizableMap<int, int> buffer(getCtx(), usage, 0, initial);

    EXPECT_EQ(usage, buffer.getUsage());
    EXPECT_EQ(initial.size(), buffer.getCapacity());
    for (auto& init : initial) {
        EXPECT_EQ(init.second, buffer.getElement(init.first));
    }
}

TEST_F(TestBufferResizableMap, ctor2) {
    auto usage = GL_DYNAMIC_COPY;
    std::size_t capacity = 50;

    buffer::ResizableMap<int, int> buffer(getCtx(), usage, capacity);

    EXPECT_EQ(usage, buffer.getUsage());
    EXPECT_EQ(capacity, buffer.getCapacity());
}

TEST_F(TestBufferResizableMap, setElement_getElement) {
    auto buffer = initBuffer();
    for (char i = 0; i < 100; ++i) {
        std::array<char, 5> value{i, 0, 0, 0, 0};
        buffer.setElement(i, value);
    }
    for (char i = 0; i < 100; ++i) {
        std::array<char, 5> value{i, 0, 0, 0, 0};
        EXPECT_EQ(value, buffer.getElement(i));
    }
}

TEST_F(TestBufferResizableMap, removeElement) {
    auto buffer = initBuffer();
    for (char i = 0; i < 100; ++i) {
        std::array<char, 5> value{i, 0, 0, 0, 0};
        buffer.setElement(i, value);
    }
    for (char i = 0; i < 100; ++i) {
        buffer.removeElement(99 - i);
    }
    EXPECT_LE(100, buffer.getCapacity());
    for (char i = 0; i < 100; ++i) {
        EXPECT_EQ(std::nullopt, buffer.getElement(i));
    }
}
