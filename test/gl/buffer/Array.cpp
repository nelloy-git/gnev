#include "gl/buffer/Array.hpp"

#include "OpenGLContext.hpp"
#include "gtest/gtest.h"

using namespace gnev::gl;

class EXPORT TestBufferArray
    : public testing::Test
    , public OpenGLContext {
public:
    GLbitfield storage_flags =
        GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
    std::size_t size = 10;
    int init = 0;

    buffer::Array<int> initBuffer() {
        buffer::Array<int> buffer(getCtx(), storage_flags, size, init);

        EXPECT_EQ(storage_flags, buffer.getStorageFlags());
        EXPECT_EQ(size, buffer.getSize());
        for (int i = 0; i < size; ++i) {
            EXPECT_EQ(init, buffer.getElement(i));
        }

        return buffer;
    };

    void expectValue(const buffer::Array<int>& buffer, std::size_t pos, int value) {
        int dst = -1199448855;
        buffer.getSubData(pos * sizeof(int), sizeof(int), &dst);
        EXPECT_EQ(dst, value);
    }
};

TEST_F(TestBufferArray, ctor) {
    auto flags = GL_MAP_READ_BIT;
    auto size = 50;
    auto init = 101010;

    buffer::Array<int> buffer(getCtx(), flags, size, init);

    EXPECT_EQ(flags, buffer.getStorageFlags());
    EXPECT_EQ(size, buffer.getSize());
    for (int i = 0; i < size; ++i) {
        expectValue(buffer, i, init);
    }
}

TEST_F(TestBufferArray, setElement) {
    auto buffer = initBuffer();
    for (int i = 0; i < size; ++i) {
        buffer.setElement(i, size - i);
    }
    for (int i = 0; i < size; ++i) {
        expectValue(buffer, i, size - i);
    }
}

TEST_F(TestBufferArray, getElement) {
    auto buffer = initBuffer();
    for (int i = 0; i < size; ++i) {
        buffer.setElement(i, size - i);
    }
    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(size - i, buffer.getElement(i));
    }
}

TEST_F(TestBufferArray, copyElement) {
    int value = 15;

    auto buffer = initBuffer();
    buffer.setElement(5, value);

    for (int i = 0; i < size; ++i) {
        buffer.copyElement(5, i);
    }
    for (int i = 0; i < size; ++i) {
        expectValue(buffer, i, value);
    }
}

TEST_F(TestBufferArray, setRange) {
    auto buffer = initBuffer();

    int first = 3;
    int last = 2;

    std::vector<int> data{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    buffer.setRange(0, first, &data[0]);
    buffer.setRange(size - last, last, &data[data.size() - last]);

    for (int i = 0; i < first; ++i) {
        expectValue(buffer, i, data[i]);
    }

    for (int i = size - last; i < size; ++i) {
        expectValue(buffer, i, data[i]);
    }
}

TEST_F(TestBufferArray, copyRange) {
    auto buffer = initBuffer();
    std::vector<int> data{10, 20, 30};

    buffer.setRange(0, data.size(), data.data());
    for (int i = 0; i + 2 * data.size() < size; i += data.size()) {
        buffer.copyRange(i, i + data.size(), data.size());
    }

    for (int i = 0; i + data.size() < size; i += data.size()) {
        for (int j = 0; j < data.size(); ++j) {
            expectValue(buffer, i + j, data[j]);
        }
    }
}

TEST_F(TestBufferArray, copyRangeOverlap) {
    auto buffer = initBuffer();
    std::vector<int> data{10, 20, 30};

    buffer.setRange(0, data.size(), data.data());
    buffer.copyRange(0, 1, data.size());
    expectValue(buffer, 0, data[0]);
    expectValue(buffer, 1, data[0]);
    expectValue(buffer, 2, data[1]);
    expectValue(buffer, 3, data[2]);
}

TEST_F(TestBufferArray, getRange) {
    auto buffer = initBuffer();

    std::vector<int> data{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    buffer.setRange(0, data.size(), data.data());
    EXPECT_EQ(data, buffer.getRange(0, data.size()));
}