#ifndef FM_LIB_FM_H_
#define FM_LIB_FM_H_

#include "BucketList.hpp"
#include <set>
#include <random>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <set>
#include <chrono>


//#include <gtest/gtest.h>

namespace fm {
    class FMAlgorithm
    {
        // friend class FMAlgorithmTest;
        // FRIEND_TEST(FMAlgorithmTest, InitialPartitionTest);
        protected:
            // random number generation requirements
            std::random_device rd;
            std::mt19937 gen;
            std::uniform_int_distribution<> dis;

            BucketList *bucketA = nullptr;
            BucketList *bucketB = nullptr;
            int numOfCells = 0;
            int numOfNets = 0;
            int pmax = 0;
            int size_A = 0;
            // the balance ratio in FM
            double ratio = 0; 
            // the balance tolerance in FM
            int k = 0;
            // the current value of the gain for each cell
            int *gain = nullptr;
            // true: the cell is free, false: the cell is locked
            bool *free = nullptr;
            // how many cells are free at current iteration
            int countOfFreeCells = 0;
            // true: cell is in partition A, false: cell is in partition B
            char *partition = nullptr;
            // distribution[0][i] : A(i), distribution[1][i] : B(i)
            int *distribution[2] = {nullptr, nullptr};
            // net to cells mapping
            std::set<int>* nets_to_cells;
            // cell to nets mapping
            std::set<int>* cells_to_nets;            
            // maximum number of iterations of the algorithm
            const int MAX_ITER;

            // is balance criterion satisfied if we move a cell?
            bool is_balance_criterion_satisfied(char from);

            // initial randomized balanced partition of cells into buckets
            void initial_partition(const std::set<int>* partition_A = nullptr);

            // initial distribution of cells w.r.t. the partition
            void initial_distribution();

            // initial gain of each cell
            void initial_gain();

            // moving a base cell and update
            void move_base_cell(int cell);

            // a single move in the algorithm
            bool single_move();

            // an iteration of the algorithm
            bool iterate();

            bool verbose = true;

        public:
            FMAlgorithm(double ratio = 0.5, int k = 0, int max_iter = 100, bool verbose = true);
            ~FMAlgorithm();

            // run the algorithm
            void run(std::string input_filename, std::string output_filename);

            // read the input file
            void read_input(std::string input_filename);

            // write the output file
            void write_output(std::string output_filename);
    };
}

#endif // FM_LIB_FM_H