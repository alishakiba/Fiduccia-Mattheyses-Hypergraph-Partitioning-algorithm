// bazel test --test_output=all //test:fm_algorithm_test --compiler=mingw-gcc

#include <gtest/gtest.h>

#include "lib/FMAlgorithm.hpp"

namespace fm {

    class FMAlgorithmTest : public ::testing::Test {
        protected:
            void SetUp() override {
                //
                this->fm = new fm::FMAlgorithm(ratio, k, max_iter);
            }

            void TearDown() override {
                delete fm;
            }

            fm::FMAlgorithm *fm;

            int max_iter = 100;
            double ratio = 0.5;
            int k = 0;
    };

    // TEST_F(FMAlgorithmTest, InitialPartitionTest) {
    //     fm->initial_partition();
    //     int size_a = 0, size_b = 0;
    //     for(int i = 0; i < fm->numOfCells; ++i) {
    //         if (fm->partition[i] == 'a') {
    //             size_a++;
    //         } else if (fm->partition[i] == 'b') {
    //             size_b++;
    //         }
    //         else {
    //             abort();
    //         }
    //     }
    //     double expected_a = fm->numOfCells * ratio;
    //     std::cerr << "size_a and b: " + std::to_string(size_a) + " " + std::to_string(size_b);;
    //     EXPECT_NE(size_a, 0) << "size_a is zero";
    //     EXPECT_NE(size_b, 0) << "size_b is zero";
    //     EXPECT_NEAR(size_a, expected_a, k) << "imbalanced: " + std::to_string(size_a) + " " + std::to_string(size_b);
    //     // EXPECT_EQ(node_->getCellID(), 123) << "cell id is not 123";
    //     // node_->setCellID(456);
    //     // EXPECT_EQ(node_->getCellID(), 456) << "cell id is not 456 after change";
    // }

    int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    }

}