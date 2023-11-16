#include "gl/buffer/CoherentStorage.hpp"

#include "OpenGLContext.hpp"
#include "gtest/gtest.h"

using namespace gnev::gl;

class EXPORT TestBufferCoherentStorage
    : public testing::Test
    , public OpenGLContext {
public:
    std::size_t capacity = 10;
    int init = 0;

    buffer::CoherentStorage<int> initBuffer() {
        buffer::CoherentStorage<int> buffer(capacity, init);

        EXPECT_EQ(capacity, buffer.getCapacity());
        for (int i = 0; i < capacity; ++i) {
            EXPECT_EQ(init, buffer[i]);
        }

        return buffer;
    };

    void expectValue(const buffer::CoherentStorage<int>& buffer,
                     std::size_t pos,
                     int value) {
        int dst = -1199448855;
        buffer.getSubData(pos * sizeof(int), sizeof(int), &dst);
        EXPECT_EQ(dst, value);
    }
};

TEST_F(TestBufferCoherentStorage, ctor) {
    {
        auto size = 50;
        auto initial_value = 101010;
        buffer::CoherentStorage<int> buffer(size, initial_value);
        EXPECT_EQ(size, buffer.getCapacity());
        for (int i = 0; i < size; ++i) {
            expectValue(buffer, i, initial_value);
        }
    }
    {
        std::initializer_list<int> initial_data = {0, 5, 1, 4, 2, 3, -10};
        buffer::CoherentStorage<int> buffer(initial_data);
        EXPECT_EQ(initial_data.size(), buffer.getCapacity());
        for (int i = 0; i < initial_data.size(); ++i) {
            expectValue(buffer, i, initial_data.begin()[i]);
        }
    }
}

TEST_F(TestBufferCoherentStorage, at) {
    auto buffer = initBuffer();
    for (int i = 0; i < capacity; ++i) {
        buffer.at(i) = capacity - i;
    }
    for (int i = 0; i < capacity; ++i) {
        EXPECT_EQ(buffer.at(i), capacity - i);
        expectValue(buffer, i, capacity - i);
    }
}

TEST_F(TestBufferCoherentStorage, squareBrackets) {
    auto buffer = initBuffer();
    for (int i = 0; i < capacity; ++i) {
        buffer[i] = capacity - i;
    }
    for (int i = 0; i < capacity; ++i) {
        EXPECT_EQ(buffer[i], capacity - i);
        expectValue(buffer, i, capacity - i);
    }
}

TEST_F(TestBufferCoherentStorage, front_back_begin_end_data) {
    auto front = -111;
    auto back = -222;

    std::initializer_list<int> initial_data = {front, 5, 1, 4, 2, 3, back};
    buffer::CoherentStorage<int> buffer(initial_data);

    EXPECT_EQ(buffer.getCapacity(), initial_data.size());
    EXPECT_EQ(buffer.front(), front);
    EXPECT_EQ(buffer.back(), back);
    EXPECT_EQ(*buffer.begin(), front);
    EXPECT_EQ(*buffer.end(), back);
    EXPECT_EQ(*buffer.data(), front);
}