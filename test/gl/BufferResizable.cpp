#include "gl/BufferResizable.hpp"

#include "OpenGLContext.hpp"
#include "gtest/gtest.h"

using namespace gnev::gl;

class EXPORT TestBufferResizable
    : public testing::Test
    , public OpenGLContext {
public:
    GLenum usage = GL_STATIC_DRAW;
    std::size_t capacity = 10;
    int init = 0;

    BufferResizable<int> initBuffer() {
        BufferResizable<int> buffer(getCtx(), usage, capacity, init);

        EXPECT_EQ(usage, buffer.getUsage());
        EXPECT_EQ(capacity, buffer.getCapacity());
        for (int i = 0; i < capacity; ++i) {
            EXPECT_EQ(init, buffer.getElement(i));
        }

        return buffer;
    };

    void expectValue(const BufferResizable<int>& buffer, std::size_t pos, int value) {
        int dst = -1199448855;
        buffer.glGetBufferSubData(pos * sizeof(int), sizeof(int), &dst);
        EXPECT_EQ(dst, value);
    }
};

TEST_F(TestBufferResizable, construct) {
    auto usage = GL_DYNAMIC_COPY;
    auto capacity = 50;
    auto init = 101010;

    BufferResizable<int> buffer(getCtx(), usage, capacity, init);

    EXPECT_EQ(usage, buffer.getUsage());
    EXPECT_EQ(capacity, buffer.getCapacity());
    for (int i = 0; i < capacity; ++i) {
        expectValue(buffer, i, init);
    }
}

TEST_F(TestBufferResizable, setElement) {
    auto buffer = initBuffer();
    for (int i = 0; i < capacity; ++i) {
        buffer.setElement(i, capacity - i);
    }
    for (int i = 0; i < capacity; ++i) {
        expectValue(buffer, i, capacity - i);
    }
}

TEST_F(TestBufferResizable, copyElement) {
    int value = 15;

    auto buffer = initBuffer();
    buffer.setElement(5, value);

    for (int i = 0; i < capacity; ++i) {
        buffer.copyElement(5, i);
    }
    for (int i = 0; i < capacity; ++i) {
        expectValue(buffer, i, value);
    }
}

TEST_F(TestBufferResizable, setRange) {
    auto buffer = initBuffer();

    int first = 3;
    int last = 2;

    std::vector<int> data{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    buffer.setRange(0, first, &data[0]);
    buffer.setRange(capacity - last, last, &data[data.size() - last]);

    for (int i = 0; i < first; ++i) {
        expectValue(buffer, i, data[i]);
    }

    for (int i = capacity - last; i < capacity; ++i) {
        expectValue(buffer, i, data[i]);
    }
}