#include <gtest/gtest.h>

#include "lib/BucketList.hpp"

class BucketListTest : public ::testing::Test {
    protected:
        void SetUp() override {
            //
            this->bucket_a = new fm::BucketList(50, 8);
        }

        void TearDown() override {
            delete bucket_a;
        }

        fm::BucketList *bucket_a;
};

TEST_F(BucketListTest, AddCellTest) {
    EXPECT_EQ(bucket_a->getMaxNumberCells(), 8) << "Error is storing C";
    EXPECT_EQ(bucket_a->getRemainingCells(), 0) << "It should be emty, but it is not at initialization";
    bucket_a->add_cell(4, 2);
    EXPECT_EQ(bucket_a->getRemainingCells(), 1) << "Why it is not counted? This is the first add";
    EXPECT_EQ(bucket_a->getMaxGain(), 2) << "max gain is not updated after first add?";
    bucket_a->add_cell(4, -1);
    EXPECT_EQ(bucket_a->getRemainingCells(), 1) << "This is an update, should not change count";
    EXPECT_EQ(bucket_a->getMaxGain(), -1) << "max gain is not updated after first update?";
    bucket_a->add_cell(5, 2);
    EXPECT_EQ(bucket_a->getRemainingCells(), 2);
    EXPECT_EQ(bucket_a->getMaxGain(), 2) << "max gain is not updated after second add?";
    bucket_a->add_cell(6, 7);
    EXPECT_EQ(bucket_a->getRemainingCells(), 3);
    EXPECT_EQ(bucket_a->getMaxGain(), 7);
    bucket_a->add_cell(2, 7);
    EXPECT_EQ(bucket_a->getRemainingCells(), 4);
    EXPECT_EQ(bucket_a->getMaxGain(), 7);
    bucket_a->add_cell(1, 7);
    EXPECT_EQ(bucket_a->getRemainingCells(), 5);
    EXPECT_EQ(bucket_a->getMaxGain(), 7);
    bucket_a->remove_cell(5);
}

// TEST_F(BucketListTest, RemoveCellTest) {
//     EXPECT_EQ(node_->getNext(), nullptr) << "next is not null";
//     EXPECT_EQ(node_->getPrev(), nullptr) << "prev is not null";
// }

// TEST_F(BucketListTest, PrevNextSetterGetterWork) {
//     EXPECT_EQ(node_->getNext(), nullptr) << "next is not null";
//     node_->setNext(node_next_);
//     EXPECT_EQ(node_->getNext()->getCellID(), 789) << "next is not 789";

//     EXPECT_EQ(node_->getPrev(), nullptr) << "prev is not null";
//     node_->setPrev(node_prev_);
//     EXPECT_EQ(node_->getPrev()->getCellID(), 111) << "next is not 111";
// }

// TEST(NodeTestAAA, Hello) {
//     EXPECT_EQ(1,1) << "Wrong";
// }

// TEST(HelloTest, BasicAssertions) {
//   // Expect two strings not to be equal.
//   EXPECT_STRNE("hello", "world");
//   // Expect equality.
//   EXPECT_EQ(7 * 6, 42);
// }


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

