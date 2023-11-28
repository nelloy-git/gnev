// #include "gl/buffer/BufStorage.hpp"

// #include "OpenGLContext.hpp"
// #include "gtest/gtest.h"

// using namespace gnev::gl;

// class EXPORT TestBufStorage
//     : public testing::Test
//     , public OpenGLContext {
// public:
//     GLbitfield storage_flags =
//         GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
//     GLuint capacity = 10;
//     int init = 0;

//     BufStorage<int> initBuffer() {
//         BufStorage<int> buffer(storage_flags, capacity, {}, init);

//         EXPECT_EQ(capacity, buffer.getCapacity());
//         EXPECT_EQ(storage_flags, buffer.getStorageFlags());
//         for (int i = 0; i < capacity; ++i) {
//             expectValue(buffer, i, init);
//         }

//         return buffer;
//     };

//     void expectValue(const BufStorage<int>& buffer, GLuint pos, int value) {
//         int dst;
//         buffer.getSubData(pos * sizeof(int), sizeof(int), &dst);
//         EXPECT_EQ(value, dst);
//     }
// };

// TEST_F(TestBufStorage, ctor) {
//     GLbitfield flags = GL_MAP_READ_BIT;
//     auto capacity = 50;
//     auto list = {5, 4, 3, 2, 1};
//     auto init = 101010;

//     BufStorage<int> buffer(flags, capacity, list, init);

//     EXPECT_EQ(flags, buffer.getStorageFlags());
//     EXPECT_EQ(capacity, buffer.getCapacity());
//     for (int i = 0; i < list.size(); ++i) {
//         expectValue(buffer, i, *(list.begin() + i));
//     }
//     for (int i = list.size(); i < capacity; ++i) {
//         expectValue(buffer, i, init);
//     }
// }

// TEST_F(TestBufStorage, Iterator_setData_getData) {
//     auto buffer = initBuffer();

//     GLuint start = 5;
//     std::vector<int> data = {11, 12, 13, 14, 15};

//     for (GLuint i = 0; i < data.size(); ++i) {
//         buffer[i + start].setData(data[i]);
//     }

//     for (GLuint i = 0; i < data.size(); ++i) {
//         EXPECT_EQ(data[i], buffer[i + start].getData());
//         expectValue(buffer, i + start, data[i]);
//     }
// }

// TEST_F(TestBufStorage, Iterator_reference) {
//     auto buffer = initBuffer();

//     GLuint start = 5;
//     std::vector<int> data = {11, 12, 13, 14, 15};

//     for (GLuint i = 0; i < data.size(); ++i) {
//         *buffer[i + start] = data[i];
//     }

//     for (GLuint i = 0; i < data.size(); ++i) {
//         EXPECT_EQ(data[i], *buffer[i + start]);
//         expectValue(buffer, i + start, data[i]);
//     }
// }
