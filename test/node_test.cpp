#include <gtest/gtest.h>

#include "lib/Node.hpp"

class NodeTest : public ::testing::Test {
    protected:
        void SetUp() override {
            //
            this->node_ = new fm::Node(123, 0);
            this->node_next_ = new fm::Node(789, -1);
            this->node_prev_ = new fm::Node(111, +1);
        }

        void TearDown() override {
            delete node_;
            delete node_prev_;
            delete node_next_;
        }

        fm::Node * node_;
        fm::Node * node_prev_;
        fm::Node * node_next_;
};

TEST_F(NodeTest, CellIDSettersGettersWork) {
    EXPECT_EQ(node_->getCellID(), 123) << "cell id is not 123";
    node_->setCellID(456);
    EXPECT_EQ(node_->getCellID(), 456) << "cell id is not 456 after change";
}

TEST_F(NodeTest, PrevNextInitiallyNull) {
    EXPECT_EQ(node_->getNext(), nullptr) << "next is not null";
    EXPECT_EQ(node_->getPrev(), nullptr) << "prev is not null";
}

TEST_F(NodeTest, PrevNextSetterGetterWork) {
    EXPECT_EQ(node_->getNext(), nullptr) << "next is not null";
    node_->setNext(node_next_);
    EXPECT_EQ(node_->getNext()->getCellID(), 789) << "next is not 789";

    EXPECT_EQ(node_->getPrev(), nullptr) << "prev is not null";
    node_->setPrev(node_prev_);
    EXPECT_EQ(node_->getPrev()->getCellID(), 111) << "next is not 111";
}

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

