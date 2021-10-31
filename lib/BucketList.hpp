#ifndef FM_LIB_BUCKET_LIST_H_
#define FM_LIB_BUCKET_LIST_H_

#include "Node.hpp"
#include <stdexcept>
#include <assert.h>

namespace fm {
    class BucketList {
        protected:
            //
            int pmax;
            int C;
            int max_gain;
            unsigned int max_gain_index;
            Node ** cell;
            Node ** gains;
            int remaining_cells;
            unsigned int gain_2_index(int gain);
            int index_2_gain(unsigned int index);
            void find_the_next_max_gain();
            void add_cell_(int cell_id, int gain);
            void remove_cell_(int cell_id);
        public:
            BucketList(int pmax, int C);
            BucketList(BucketList& bl);
            ~BucketList();
            void add_cell(int cell_id, int gain);
            void remove_cell(int cell_id);
            int getMaxGain();
            bool isEmpty();
            int getRemainingCells();
            int getMaxNumberCells();
    };
}

#endif // FM_LIB_BUCKET_LIST_H_
