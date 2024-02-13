#include "util/IndexManager.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace gnev;

class IndexManagerTest : public testing::Test {
protected:
    Ref<IndexManager> manager_sut = IndexManager::Make(10);
    IndexManager::FreeCallback default_free_callback =
        [](const IndexGroup&) {};
    IndexManager::MoveCallback default_move_callback =
        [](unsigned src, unsigned dst, unsigned count) {};
};

TEST_F(IndexManagerTest, reserve_FirstReserve) {
    unsigned count = 4;
    IndexGroup expect{.first = 0, .count = count};

    EXPECT_EQ(expect, manager_sut->reserve(count, default_free_callback).value());
}

TEST_F(IndexManagerTest, reserve_Release) {
    unsigned count = 2;

    IndexGroup expect1{.first = 0, .count = count};
    auto gr1 = manager_sut->reserve(count, default_free_callback).value();
    EXPECT_EQ(expect1, gr1);

    IndexGroup expect2{.first = count, .count = count};
    auto gr2 = manager_sut->reserve(count, default_free_callback).value();
    EXPECT_EQ(expect2, gr2);

    IndexGroup expect3{.first = 2 * count, .count = count};
    auto gr3 = manager_sut->reserve(count, default_free_callback).value();
    EXPECT_EQ(expect3, gr3);

    // Free group
    gr2 = MakeSharable<IndexGroup>();

    IndexGroup expect4{.first = count, .count = 1};
    auto gr4 = manager_sut->reserve(1, default_free_callback).value();
    EXPECT_EQ(expect4, gr4);

    IndexGroup expect5{.first = 3 * count, .count = 2};
    auto gr5 = manager_sut->reserve(2, default_free_callback).value();
    EXPECT_EQ(expect5, gr5);

    IndexGroup expect6{.first = count + 1, .count = 1};
    auto gr6 = manager_sut->reserve(1, default_free_callback).value();
    EXPECT_EQ(expect6, gr6);
}

TEST_F(IndexManagerTest, reserve_DeleteCallback) {
    unsigned count = 2;
    IndexGroup expect{.first = 0, .count = count};

    testing::MockFunction<void(const IndexGroup& gr)> deleter_callback;
    EXPECT_CALL(deleter_callback, Call(testing::_)).WillOnce(testing::Return());

    auto gr = manager_sut->reserve(count, deleter_callback.AsStdFunction()).value();
    EXPECT_EQ(expect, gr);
}

TEST_F(IndexManagerTest, optimize_Simple) {
    unsigned count = 2;

    IndexGroup expect1{.first = 0, .count = count};
    auto gr1 = manager_sut->reserve(count, default_free_callback).value();
    EXPECT_EQ(expect1, gr1);

    IndexGroup expect2{.first = count, .count = count};
    auto gr2 = manager_sut->reserve(count, default_free_callback).value();
    EXPECT_EQ(expect2, gr2);

    IndexGroup expect3{.first = 2 * count, .count = count};
    auto gr3 = manager_sut->reserve(count, default_free_callback).value();
    EXPECT_EQ(expect3, gr3);

    // Free group
    gr2 = MakeSharable<IndexGroup>();

    manager_sut->optimize(default_move_callback);
    EXPECT_EQ(expect1, gr1);
    IndexGroup expect_moved{.first = count, .count = count};
    EXPECT_EQ(expect_moved, gr3);
}

TEST_F(IndexManagerTest, optimize_MoveCallback) {
    unsigned count = 2;

    IndexGroup expect1{.first = 0, .count = count};
    auto gr1 = manager_sut->reserve(count, default_free_callback).value();
    EXPECT_EQ(expect1, gr1);

    IndexGroup expect2{.first = count, .count = count};
    auto gr2 = manager_sut->reserve(count, default_free_callback).value();
    EXPECT_EQ(expect2, gr2);

    IndexGroup expect3{.first = 2 * count, .count = 1};
    auto gr3 = manager_sut->reserve(1, default_free_callback).value();
    EXPECT_EQ(expect3, gr3);

    // Free group
    gr2 = MakeSharable<IndexGroup>();

    testing::MockFunction<void(unsigned src, unsigned dst, unsigned count)> move_callback;
    EXPECT_CALL(move_callback, Call(expect3.first, expect2.first, expect3.count)).WillOnce(testing::Return());

    manager_sut->optimize(move_callback.AsStdFunction());
    EXPECT_EQ(expect1, gr1);
    IndexGroup expect_moved{.first = count, .count = 1};
    EXPECT_EQ(expect_moved, gr3);
}