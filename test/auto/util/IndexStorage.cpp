// #include "util/IndexStorage.hpp"

// #include "gtest/gtest.h"

// using namespace gnev;

// class IndexStorageTest : public testing::Test {
// public:
// };

// TEST_F(IndexStorageTest, useIndex) {
//     {
//         IndexStorage storage(10);
//         EXPECT_EQ(0, storage.useIndex());
//         EXPECT_EQ(1, storage.useIndex());
//         EXPECT_EQ(2, storage.useIndex());
//         EXPECT_EQ(3, storage.useIndex());
//     }
//     {
//         IndexStorage storage(10, 5);
//         EXPECT_EQ(5, storage.useIndex());
//         EXPECT_EQ(6, storage.useIndex());
//         EXPECT_EQ(7, storage.useIndex());
//         EXPECT_EQ(8, storage.useIndex());
//     }
// }

// TEST_F(IndexStorageTest, freeIndex) {
//     IndexStorage storage(10);
//     EXPECT_EQ(10, storage.countFree());
//     EXPECT_EQ(0, storage.useIndex());
//     EXPECT_EQ(1, storage.useIndex());
//     EXPECT_EQ(2, storage.useIndex());
//     EXPECT_EQ(3, storage.useIndex());
//     EXPECT_EQ(6, storage.countFree());

//     EXPECT_TRUE(storage.freeIndex(1));
//     EXPECT_EQ(7, storage.countFree());
//     EXPECT_TRUE(storage.freeIndex(3));
//     EXPECT_EQ(8, storage.countFree());
//     EXPECT_FALSE(storage.freeIndex(1));
//     EXPECT_EQ(8, storage.countFree());

//     EXPECT_EQ(3, storage.useIndex());
//     EXPECT_EQ(1, storage.useIndex());
//     EXPECT_EQ(6, storage.countFree());
// }

// TEST_F(IndexStorageTest, isUsed) {
//     IndexStorage storage(10);
//     EXPECT_EQ(10, storage.countFree());

//     EXPECT_FALSE(storage.isUsed(0));
//     EXPECT_EQ(0, storage.useIndex());
//     EXPECT_TRUE(storage.isUsed(0));

//     EXPECT_FALSE(storage.isUsed(1));
//     EXPECT_EQ(1, storage.useIndex());
//     EXPECT_TRUE(storage.isUsed(1));

//     EXPECT_FALSE(storage.isUsed(2));
//     EXPECT_EQ(2, storage.useIndex());
//     EXPECT_TRUE(storage.isUsed(2));

//     EXPECT_TRUE(storage.freeIndex(1));
//     EXPECT_FALSE(storage.isUsed(1));
// }