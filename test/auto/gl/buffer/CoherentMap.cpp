#include "gl/buffer/CoherentMap.hpp"

#include "OpenGLContext.hpp"
#include "gtest/gtest.h"

using namespace gnev::gl;

class EXPORT TestBufferCoherentMap
    : public testing::Test
    , public OpenGLContext {
public:
    std::size_t capacity = 10;
    int init = 0;

    buffer::CoherentMap<int, int> initBuffer() {
        buffer::CoherentMap<int, int> buffer(capacity, {}, init);
        EXPECT_EQ(capacity, buffer.getCapacity());
        return buffer;
    };
};

TEST_F(TestBufferCoherentMap, ctor) {
    auto size = 50;
    std::initializer_list<std::pair<int, int>> initial_data = {{19, 9},
                                                               {28, 8},
                                                               {37, 7},
                                                               {46, 6},
                                                               {55, 5}};
    buffer::CoherentMap<int, int> buffer(size, initial_data, 0, true);
    EXPECT_EQ(buffer.getSize(), initial_data.size());
    EXPECT_EQ(buffer.getCapacity(), size);
    for (auto& pair : initial_data) {
        EXPECT_EQ(buffer[pair.first], pair.second);
    }
}

TEST_F(TestBufferCoherentMap, squareBrackets) {
    auto buffer = initBuffer();
    for (int i = 0; i < capacity; ++i) {
        buffer[i] = capacity - i;
    }
    for (int i = 0; i < capacity; ++i) {
        EXPECT_EQ(buffer[i], capacity - i);
    }
}

TEST_F(TestBufferCoherentMap, isEmpty) {
    auto buffer = initBuffer();
    EXPECT_EQ(buffer.isEmpty(), true);
    buffer[3] = 3;
    EXPECT_EQ(buffer.isEmpty(), false);
}

TEST_F(TestBufferCoherentMap, getSize) {
    auto buffer = initBuffer();
    EXPECT_EQ(buffer.getSize(), 0);
    buffer[3] = 3;
    buffer[5] = 5;
    buffer[5] = 3;
    EXPECT_EQ(buffer.getSize(), 2);
}

TEST_F(TestBufferCoherentMap, contains) {
    auto buffer = initBuffer();
    EXPECT_EQ(buffer.getSize(), 0);
    buffer[3] = 3;
    buffer[5] = 5;
    buffer[5] = 3;
    EXPECT_EQ(buffer.contains(3), true);
    EXPECT_EQ(buffer.contains(5), true);
    EXPECT_EQ(buffer.contains(1), false);
}

TEST_F(TestBufferCoherentMap, extract) {
    auto buffer = initBuffer();
    EXPECT_EQ(buffer.getSize(), 0);
    buffer[3] = 3;
    buffer[5] = 5;
    buffer.extract(5);
    EXPECT_EQ(buffer.contains(3), true);
    EXPECT_EQ(buffer.contains(5), false);
}
