#include "gl/buffer/Resizable.hpp"

#include "OpenGLContext.hpp"
#include "gtest/gtest.h"

using namespace gnev::gl;

class EXPORT TestBufferResizable
    : public testing::Test
    , public OpenGLContext {
public:
    GLenum usage = GL_STATIC_DRAW;
    std::size_t init_capacity = 10;
    int init = 0;

    buffer::Resizable<int> initBuffer() {
        buffer::Resizable<int> buffer(getCtx(), usage, init_capacity, init);

        EXPECT_EQ(usage, buffer.getUsage());
        EXPECT_EQ(init_capacity, buffer.getCapacity());
        for (int i = 0; i < init_capacity; ++i) {
            EXPECT_EQ(init, buffer.getElement(i));
        }

        return buffer;
    };

    void expectValue(const buffer::Resizable<int>& buffer, std::size_t pos, int value) {
        int dst = -1199448855;
        buffer.glGetBufferSubData(pos * sizeof(int), sizeof(int), &dst);
        EXPECT_EQ(dst, value);
    }
};

TEST_F(TestBufferResizable, ctor) {
    auto usage = GL_DYNAMIC_COPY;
    auto capacity = 50;
    auto init = 101010;

    buffer::Resizable<int> buffer(getCtx(), usage, capacity, init);

    EXPECT_EQ(usage, buffer.getUsage());
    EXPECT_EQ(capacity, buffer.getCapacity());
    for (int i = 0; i < capacity; ++i) {
        expectValue(buffer, i, init);
    }
}

TEST_F(TestBufferResizable, setElement) {
    auto buffer = initBuffer();
    for (int i = 0; i < init_capacity; ++i) {
        buffer.setElement(i, init_capacity - i);
    }
    for (int i = 0; i < init_capacity; ++i) {
        expectValue(buffer, i, init_capacity - i);
    }
}

TEST_F(TestBufferResizable, getElement) {
    auto buffer = initBuffer();
    for (int i = 0; i < init_capacity; ++i) {
        buffer.setElement(i, init_capacity - i);
    }
    for (int i = 0; i < init_capacity; ++i) {
        EXPECT_EQ(init_capacity - i, buffer.getElement(i));
    }
}

TEST_F(TestBufferResizable, copyElement) {
    int value = 15;

    auto buffer = initBuffer();
    buffer.setElement(5, value);

    for (int i = 0; i < init_capacity; ++i) {
        buffer.copyElement(5, i);
    }
    for (int i = 0; i < init_capacity; ++i) {
        expectValue(buffer, i, value);
    }
}

TEST_F(TestBufferResizable, setRange) {
    auto buffer = initBuffer();

    int first = 3;
    int last = 2;

    std::vector<int> data{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    buffer.setRange(0, first, &data[0]);
    buffer.setRange(init_capacity - last, last, &data[data.size() - last]);

    for (int i = 0; i < first; ++i) {
        expectValue(buffer, i, data[i]);
    }

    for (int i = init_capacity - last; i < init_capacity; ++i) {
        expectValue(buffer, i, data[i]);
    }
}

TEST_F(TestBufferResizable, copyRange) {
    auto buffer = initBuffer();
    std::vector<int> data{10, 20, 30};

    buffer.setRange(0, data.size(), data.data());
    for (int i = 0; i + 2 * data.size() < init_capacity; i += data.size()) {
        buffer.copyRange(i, i + data.size(), data.size());
    }

    for (int i = 0; i + data.size() < init_capacity; i += data.size()) {
        for (int j = 0; j < data.size(); ++j) {
            expectValue(buffer, i + j, data[j]);
        }
    }
}

TEST_F(TestBufferResizable, copyRangeOverlap) {
    auto buffer = initBuffer();
    std::vector<int> data{10, 20, 30};

    buffer.setRange(0, data.size(), data.data());
    buffer.copyRange(0, 1, data.size());
    expectValue(buffer, 0, data[0]);
    expectValue(buffer, 1, data[0]);
    expectValue(buffer, 2, data[1]);
    expectValue(buffer, 3, data[2]);
}

TEST_F(TestBufferResizable, getRange) {
    auto buffer = initBuffer();

    std::vector<int> data{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    buffer.setRange(0, data.size(), data.data());
    EXPECT_EQ(data, buffer.getRange(0, data.size()));
}

TEST_F(TestBufferResizable, setCapacity) {
    auto buffer = initBuffer();

    std::vector<int> data{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    std::vector<int> zeroes(init_capacity, int{});

    buffer.setRange(0, data.size(), data.data());
    buffer.setCapacity(2 * init_capacity);
    EXPECT_EQ(data, buffer.getRange(0, init_capacity));
    EXPECT_EQ(zeroes, buffer.getRange(init_capacity, init_capacity));

    buffer.setCapacity(init_capacity);
    EXPECT_EQ(data, buffer.getRange(0, init_capacity));
}

TEST_F(TestBufferResizable, getCapacity) {
    auto buffer = initBuffer();

    buffer.setCapacity(2 * init_capacity);

    GLint buffer_size = 0;
    buffer.glGetBufferParameteriv(GL_BUFFER_SIZE, &buffer_size);
    EXPECT_EQ(2 * init_capacity, buffer_size / sizeof(int));
}

TEST_F(TestBufferResizable, setUsage) {
    auto buffer = initBuffer();

    std::vector<int> data{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    buffer.setRange(0, data.size(), data.data());

    buffer.setUsage(GL_DYNAMIC_DRAW);

    GLenum usage;
    buffer.glGetBufferParameteriv(GL_BUFFER_USAGE, reinterpret_cast<GLint*>(&usage));
    EXPECT_EQ(GL_DYNAMIC_DRAW, usage);
    EXPECT_EQ(data, buffer.getRange(0, init_capacity));
}

TEST_F(TestBufferResizable, getUsage) {
    auto buffer = initBuffer();

    buffer.setUsage(GL_DYNAMIC_DRAW);
    EXPECT_EQ(GL_DYNAMIC_DRAW, buffer.getUsage());
}