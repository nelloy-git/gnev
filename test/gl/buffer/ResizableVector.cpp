#include "gl/buffer/ResizableVector.hpp"

#include "OpenGLContext.hpp"
#include "gtest/gtest.h"

using namespace gnev::gl;

class EXPORT TestBufferResizableVector
    : public testing::Test
    , public OpenGLContext {
public:
    GLenum usage = GL_STATIC_DRAW;
    std::size_t init_size = 10;
    int init = 0;

    buffer::ResizableVector<int> initBuffer() {
        buffer::ResizableVector<int> buffer(getCtx(), usage, init_size, init);

        EXPECT_EQ(usage, buffer.getUsage());
        EXPECT_EQ(init_size, buffer.getSize());
        for (int i = 0; i < init_size; ++i) {
            EXPECT_EQ(init, buffer.getElement(i));
        }

        return buffer;
    };

    std::vector<int> initStdVector() { return std::vector<int>(init_size, init); };

    void expectValue(const buffer::ResizableVector<int>& buffer,
                     std::size_t pos,
                     int value) {
        int dst = -1199448855;
        buffer.getSubData(pos * sizeof(int), sizeof(int), &dst);
        EXPECT_EQ(dst, value);
    }
};

TEST_F(TestBufferResizableVector, ctor) {
    auto usage = GL_DYNAMIC_COPY;
    auto size = 50;
    auto init = 101010;

    buffer::ResizableVector<int> buffer(getCtx(), usage, size, init);

    EXPECT_EQ(usage, buffer.getUsage());
    EXPECT_EQ(size, buffer.getSize());
    for (int i = 0; i < size; ++i) {
        expectValue(buffer, i, init);
    }
}

TEST_F(TestBufferResizableVector, setElement) {
    auto expected = initStdVector();
    auto buffer = initBuffer();

    for (int i = 0; i < init_size; ++i) {
        buffer.setElement(i, init_size - i);
        expected[i] = init_size - i;
    }
    for (int i = 0; i < init_size; ++i) {
        expectValue(buffer, i, expected[i]);
    }
}

TEST_F(TestBufferResizableVector, copyElement) {
    int value = 15;

    auto buffer = initBuffer();
    buffer.setElement(5, value);

    for (int i = 0; i < init_size; ++i) {
        buffer.copyElement(5, i);
    }
    for (int i = 0; i < init_size; ++i) {
        expectValue(buffer, i, value);
    }
}

TEST_F(TestBufferResizableVector, insertElement) {
    int n = 3;
    auto expected = initStdVector();
    auto buffer = initBuffer();

    for (int i = 0; i < n; ++i) {
        expected.insert(expected.begin() + 5 + i, n - i);
        buffer.insertElement(5 + i, n - i);
    }
    EXPECT_EQ(expected.size(), buffer.getSize());
    for (int i = 0; i < expected.size(); ++i) {
        expectValue(buffer, i, expected[i]);
    }
}

TEST_F(TestBufferResizableVector, pushElementBack) {
    int n = 3;
    int value = 15;

    auto expected = initStdVector();
    auto buffer = initBuffer();

    for (int i = 0; i < n; ++i) {
        buffer.pushElementBack(value);
        expected.push_back(value);
    }

    EXPECT_EQ(expected.size(), buffer.getSize());
    for (int i = 0; i < expected.size(); ++i) {
        expectValue(buffer, i, expected[i]);
    }
}

TEST_F(TestBufferResizableVector, eraseElement) {
    auto expected = initStdVector();
    auto buffer = initBuffer();

    for (int i = 0; i < expected.size(); ++i) {
        expected[i] = i;
        buffer.setElement(i, i);
    }
    EXPECT_EQ(expected.size(), buffer.getSize());
    for (int i = 0; i < expected.size(); ++i) {
        expectValue(buffer, i, expected[i]);
    }

    int i = 1;
    while (i < expected.size()) {
        expected.erase(expected.begin() + i);
        buffer.eraseElement(i);
        ++i;
    }
    EXPECT_EQ(expected.size(), buffer.getSize());
    EXPECT_EQ(expected, buffer.getRange(0, buffer.getSize()));
}

TEST_F(TestBufferResizableVector, setRange) {
    auto buffer = initBuffer();

    int first = 3;
    int last = 2;

    std::vector<int> data{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    buffer.setRange(0, first, &data[0]);
    buffer.setRange(init_size - last, last, &data[data.size() - last]);

    for (int i = 0; i < first; ++i) {
        expectValue(buffer, i, data[i]);
    }

    for (int i = init_size - last; i < init_size; ++i) {
        expectValue(buffer, i, data[i]);
    }
}

TEST_F(TestBufferResizableVector, copyRange) {
    auto buffer = initBuffer();
    std::vector<int> data{10, 20, 30};

    buffer.setRange(0, data.size(), data.data());
    for (int i = 0; i + 2 * data.size() < init_size; i += data.size()) {
        buffer.copyRange(i, i + data.size(), data.size());
    }

    for (int i = 0; i + data.size() < init_size; i += data.size()) {
        for (int j = 0; j < data.size(); ++j) {
            expectValue(buffer, i + j, data[j]);
        }
    }
}

TEST_F(TestBufferResizableVector, copyRangeOverlap) {
    auto buffer = initBuffer();
    std::vector<int> data{10, 20, 30};

    buffer.setRange(0, data.size(), data.data());
    buffer.copyRange(0, 1, data.size());
    expectValue(buffer, 0, data[0]);
    expectValue(buffer, 1, data[0]);
    expectValue(buffer, 2, data[1]);
    expectValue(buffer, 3, data[2]);
}

TEST_F(TestBufferResizableVector, getRange) {
    auto buffer = initBuffer();

    std::vector<int> data{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    buffer.setRange(0, data.size(), data.data());
    EXPECT_EQ(data, buffer.getRange(0, data.size()));
}

TEST_F(TestBufferResizableVector, insertRange) {
    int n = 3;
    auto expected = initStdVector();
    auto buffer = initBuffer();

    std::vector<int> data = {30, 20, 10};

    expected.insert(expected.begin(), data.begin(), data.end());
    buffer.insertRange(0, data.size(), data.data());
    EXPECT_EQ(expected, buffer.getRange(0, buffer.getSize()));
}

TEST_F(TestBufferResizableVector, pushRangeBack) {
    int n = 3;
    auto expected = initStdVector();
    auto buffer = initBuffer();

    std::vector<int> data = {30, 20, 10};

    expected.insert(expected.end(), data.begin(), data.end());
    buffer.insertRange(buffer.getSize(), data.size(), data.data());
    EXPECT_EQ(expected, buffer.getRange(0, buffer.getSize()));
}

TEST_F(TestBufferResizableVector, eraseRange) {
    auto expected = initStdVector();
    auto buffer = initBuffer();

    for (int i = 0; i < expected.size(); ++i) {
        expected[i] = i;
        buffer.setElement(i, i);
    }
    EXPECT_EQ(expected.size(), buffer.getSize());
    for (int i = 0; i < expected.size(); ++i) {
        expectValue(buffer, i, expected[i]);
    }

    int n = 4;
    expected.erase(expected.begin(), expected.begin() + n);
    buffer.eraseRange(0, n);
    EXPECT_EQ(expected, buffer.getRange(0, buffer.getSize()));
}

TEST_F(TestBufferResizableVector, getSize) {
    auto buffer = initBuffer();
    EXPECT_EQ(init_size, buffer.getSize());

    std::vector<int> data{1, 2, 3};
    buffer.pushRangeBack(data.size(), data.data());

    EXPECT_EQ(init_size + data.size(), buffer.getSize());
}

TEST_F(TestBufferResizableVector, reserve) {
    auto buffer = initBuffer();
    EXPECT_EQ(init_size, buffer.getCapacity());
    buffer.reserve(1);
    EXPECT_EQ(init_size, buffer.getCapacity());
    buffer.reserve(2 * init_size);
    EXPECT_EQ(2 * init_size, buffer.getCapacity());
}

TEST_F(TestBufferResizableVector, shrinkToFit) {
    int n = 3;
    auto buffer = initBuffer();
    EXPECT_EQ(init_size, buffer.getCapacity());
    buffer.eraseRange(0, n);
    EXPECT_EQ(init_size, buffer.getCapacity());
    EXPECT_EQ(init_size - n, buffer.getSize());
    buffer.shrinkToFit();
    EXPECT_EQ(init_size - n, buffer.getCapacity());
    EXPECT_EQ(init_size - n, buffer.getSize());
}

TEST_F(TestBufferResizableVector, clear) {
    auto buffer = initBuffer();
    EXPECT_EQ(init_size, buffer.getCapacity());
    EXPECT_EQ(init_size, buffer.getSize());
    buffer.clear();
    EXPECT_EQ(init_size, buffer.getCapacity());
    EXPECT_EQ(0, buffer.getSize());
}