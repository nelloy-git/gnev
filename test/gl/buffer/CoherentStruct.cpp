#include "gl/buffer/CoherentStruct.hpp"

#include "OpenGLContext.hpp"
#include "gtest/gtest.h"

using namespace gnev::gl;

class EXPORT TestBufferCoherentStruct
    : public testing::Test
    , public OpenGLContext {
public:
    struct TestStruct {
        bool operator==(const TestStruct&) const = default;

        int int_val = 5;
        char char_val[5] = "abcd";
    };

    std::size_t capacity = 10;
    int init = 0;

    buffer::CoherentStruct<TestStruct> initBuffer() {
        buffer::CoherentStruct<TestStruct> buffer(TestStruct{}, false);
        EXPECT_EQ(*buffer, TestStruct{});
        return buffer;
    };
};

TEST_F(TestBufferCoherentStruct, ctor) {
    {
        TestStruct initial_data{.int_val = 15, .char_val = "qwer"};
        buffer::CoherentStruct<TestStruct> buffer(initial_data, false);
        EXPECT_EQ(*buffer, initial_data);
    }
    {
        TestStruct initial_data{.int_val = 15, .char_val = "aaaa"};
        buffer::CoherentStruct<TestStruct> buffer(initial_data, true);
        EXPECT_EQ(*buffer, initial_data);
    }
}

TEST_F(TestBufferCoherentStruct, operatorArrow) {
    TestStruct initial_data{.int_val = 15, .char_val = "qwer"};
    buffer::CoherentStruct<TestStruct> buffer(initial_data, false);
    buffer->int_val++;
    initial_data.int_val++;
    EXPECT_EQ(*buffer, initial_data);
}

TEST_F(TestBufferCoherentStruct, operatorDereference) {
    TestStruct initial_data{.int_val = 15, .char_val = "qwer"};
    buffer::CoherentStruct<TestStruct> buffer(initial_data, false);
    (*buffer).int_val++;
    initial_data.int_val++;
    EXPECT_EQ(*buffer, initial_data);
}

TEST_F(TestBufferCoherentStruct, get) {
    TestStruct initial_data{.int_val = 15, .char_val = "qwer"};
    buffer::CoherentStruct<TestStruct> buffer(initial_data, false);
    buffer.get()->char_val[0] = 'a';
    initial_data.char_val[0] = 'a';
    EXPECT_EQ(*buffer, initial_data);
}