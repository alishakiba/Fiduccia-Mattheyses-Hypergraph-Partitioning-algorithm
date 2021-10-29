#ifndef FM_ALGORITHM_HEADER_H
#define FM_ALGORITHM_HEADER_H

#include "BucketListStructure.h"
#include "BucketListNode.h"
#include <set>
#include <time.h>
#include <cstdlib>
#include <string>
#include <fstream>
using namespace std;

class FMAlgorithm
{
private:
    bool *partition;  // true: partition A, false: partition B
    bool *free_cells; // true: is free, false: is locked
    int number_of_cells;
    int number_of_nets;
    int pmax;
    int *gain; // the curret value of gain of a cell
    // distribution[0][i] : A(i)
    // distribution[1][i] : B(i)
    int *distribution[2];
    double ratio;
    set<int> *nets_to_cells;
    set<int> *cells_to_nets;
    BucketListStructure *bucket_structure_a;
    BucketListStructure *bucket_structure_b;
    int size_A;
    int seed;
    int k; // balance tolerance
    int count_free_cells;
    int MAX_ITER;

protected:
    bool is_balance_criterion_valid(int from)
    {
        // from == 0 : A
        // from == 1 : B
        // check whether the balancing criterion can be maintained
        // simulting the move by considering the move from A to B
        int size = ((from == 0) ? (this->size_A - 1) : (this->number_of_cells - this->size_A - 1));
        // cerr << "from: " << from << ",size:" << size << ","
        //      << "size_A:" << this->size_A << endl;
        // cerr << this->number_of_cells * this->ratio - this->k << "<=" << size << " && "
        //      << size << "<=" << this->number_of_cells * this->ratio + this->k << endl;
        if (this->number_of_cells * this->ratio - this->k <= size && size <= this->number_of_cells * this->ratio + this->k)
        {
            // we can move from A without violating the balance criterion
            return true;
        }
        else
        {
            // the balancing criterion is violated if we move a cell
            return false;
        }
    }
    void initial_partition(const set<int> *partition_A = nullptr)
    {
        this->partition = new bool[this->number_of_cells];
        this->size_A = 0;
        for (int i = 0; i < this->number_of_cells; ++i)
        {
            if (i < this->number_of_cells / 2)
            {
                this->partition[i] = false;
            }
            else
            {
                this->partition[i] = true;
                this->size_A++;
            }
        }
        // return;
        /*
        if (partition_A == nullptr) {
            // do a random partition
            for(int i = 0; i < this->number_of_cells; ++i) {
                auto rand_value = rand() / (double) RAND_MAX;
                cerr << rand_value << endl;
                if (rand_value > 0.5) {
                    this->partition[i] = true;
                    this->size_A++;
                }
            }
        }
        else {
            for(auto cell: *partition_A) {
                this->partition[cell] = true;
            }
            this->size_A = partition_A->size();
        }
        */
        cerr << "Initial partition:" << endl;
        for (int i = 0; i < this->number_of_cells; ++i)
        {
            cerr << i << ":" << (this->partition[i] ? '0' : '1') << endl;
        }
        cerr << "size_a: " << this->size_A << ",#cells: " << this->number_of_cells << endl;
    }
    // this method computes the initial distribution
    // of the nets in the graph w.r.t. the initial
    // bi-partition
    void compute_initial_distribution()
    {
        for (int i = 0; i < this->number_of_nets; ++i)
        {
            int A = 0;
            int B = 0;
            for (auto cell : this->nets_to_cells[i])
            {
                if (this->partition[cell])
                {
                    A++;
                }
                else
                {
                    B++;
                }
            }
            this->distribution[0][i] = A;
            this->distribution[1][i] = B;
        }
    }
    // this method computes the initial gain
    // of the nets in the graph w.r.t. the initial
    // bi-partition
    void compute_initial_gain()
    {
        this->gain = new int[this->number_of_cells];
        // cerr << "*" << endl;
        for (int i = 0; i < this->number_of_cells; ++i)
        {
            // computing gain for cell i
            this->gain[i] = 0;
            for (auto net : cells_to_nets[i])
            {
                // for each net which cell i belongs to
                if (partition[i])
                {
                    // cell i belongs to partition A,
                    // so F=A and T=B according to the
                    // terminology of the paper
                    if (this->distribution[0][i] == 1)
                    {
                        this->gain[i]++;
                    }
                    if (this->distribution[1][i] == 0)
                    {
                        this->gain[i]--;
                    }
                }
                else
                {
                    // cell i belongs to partition B,
                    // so F=B and T=A according to the
                    // terminology of the paper
                    if (this->distribution[1][i] == 1)
                    {
                        this->gain[i]++;
                    }
                    if (this->distribution[0][i] == 0)
                    {
                        this->gain[i]--;
                    }
                }
            }
            // adding cell i to the BucketListStructure for
            // corresponding partition

            if (partition[i])
            {
                // cell i belongs to partition A
                this->bucket_structure_a->add_cell_to_end(i, this->gain[i]);
                // cerr << "*" << endl;
            }
            else
            {
                // cell i belongs to partition B
                this->bucket_structure_b->add_cell_to_end(i, this->gain[i]);
                // cerr << "*" << endl;
            }
        }
    }
    // move base cell and update
    void move_base_cell_and_update(int cell)
    {
        int T, F;
        if (this->partition[cell])
        {
            // this cell belongs to partition A
            // so, F=A and T=B
            T = 1;
            F = 0;
        }
        else
        {
            // this cell belongs to partition B
            // so, F=B and T=A
            T = 0;
            F = 1;
        }
        // lock the base cell
        this->free_cells[cell] = false;
        this->count_free_cells--;
        // change the base cell's partition
        this->partition[cell] = !this->partition[cell];
        for (auto net : this->cells_to_nets[cell])
        {
            // check critical nets
            if (distribution[T][net] == 0)
            {
                // increment gains of all free cells on net
                for (auto free_cell : nets_to_cells[net])
                {
                    if (this->free_cells[free_cell])
                    {
                        // it is a free cell
                        this->gain[free_cell]++;
                        if (this->partition[free_cell])
                        {
                            // it belongs to partition A
                            this->bucket_structure_a->add_cell_to_front(free_cell, this->gain[free_cell]);
                        }
                        else
                        {
                            // it belongs to partition B
                            this->bucket_structure_b->add_cell_to_front(free_cell, this->gain[free_cell]);
                        }
                    }
                }
            }
            else if (distribution[T][net] == 1)
            {
                // decrement gain of the only free cell on partition T
                for (auto free_cell : nets_to_cells[net])
                {
                    if (this->free_cells[free_cell])
                    {
                        if (this->partition[free_cell] && T == 0)
                        {
                            // it belongs to partition A
                            this->gain[free_cell]--;
                            this->bucket_structure_a->add_cell_to_front(free_cell, this->gain[free_cell]);
                        }
                        else if (this->partition[free_cell] && T == 1)
                        {
                            // it belongs to partition B
                            this->gain[free_cell]--;
                            this->bucket_structure_b->add_cell_to_front(free_cell, this->gain[free_cell]);
                        }
                    }
                }
            }
            // change the net distribution to reflect the move
            this->distribution[F][net]--;
            this->distribution[T][net]++;
            // check critical nets after the move
            if (distribution[F][net] == 0)
            {
                // decrement gains of all free cells on net
                for (auto free_cell : nets_to_cells[net])
                {
                    if (this->free_cells[free_cell])
                    {
                        // it is a free cell
                        this->gain[free_cell]--;
                        if (this->partition[free_cell])
                        {
                            // it belongs to partition A
                            this->bucket_structure_a->add_cell_to_front(free_cell, this->gain[free_cell]);
                        }
                        else
                        {
                            // it belongs to partition B
                            this->bucket_structure_b->add_cell_to_front(free_cell, this->gain[free_cell]);
                        }
                    }
                }
            }
            else if (distribution[F][net] == 1)
            {
                // decrement gain of the only free cell on partition F
                for (auto free_cell : nets_to_cells[net])
                {
                    if (this->free_cells[free_cell])
                    {
                        if (this->partition[free_cell] && F == 0)
                        {
                            // it belongs to partition A
                            this->gain[free_cell]++;
                            this->bucket_structure_a->add_cell_to_front(free_cell, this->gain[free_cell]);
                        }
                        else if (this->partition[free_cell] && F == 1)
                        {
                            // it belongs to partition B
                            this->gain[free_cell]++;
                            this->bucket_structure_b->add_cell_to_front(free_cell, this->gain[free_cell]);
                        }
                    }
                }
            }
        }
        if (F == 0)
        {
            this->size_A--;
        }
        else
        {
            this->size_A++;
        }
    }
    // an iteration of the algorithm
    bool iterate()
    {
        // cerr << "entered iterate" << endl;
        if (this->count_free_cells == 0)
        {
            return false;
        }
        // identifying the base cell
        int max_gain_a = this->bucket_structure_a->max_gain();
        int max_gain_b = this->bucket_structure_b->max_gain();
        cerr << "gains:" << max_gain_a << " " << max_gain_b << endl;
        int base_cell = -1;
        if (max_gain_a >= max_gain_b && this->is_balance_criterion_valid(0))
        {
            // we can move from A to B without violating the balance criterion
            base_cell = this->bucket_structure_a->get_cell_with_max_gain_end();
        }
        else if (max_gain_a <= max_gain_b && this->is_balance_criterion_valid(1))
        {
            // we can move from B to A without violating the balance criterion
            base_cell = this->bucket_structure_b->get_cell_with_max_gain_end();
        }
        else if (this->is_balance_criterion_valid(0)) {
            base_cell = this->bucket_structure_a->get_cell_with_max_gain_end();
        }
        else if (this->is_balance_criterion_valid(1)) {
            base_cell = this->bucket_structure_b->get_cell_with_max_gain_end();
        }
        else
        {
            // no further moves are possible
            cerr << "no further moves are possible due to violation of balance criterion." << endl;
            return false;
        }
        this->move_base_cell_and_update(base_cell);
        return true;
    }

public:
    FMAlgorithm(double ratio, int MAX_ITER, int k)
    {
        // TODO: initial partition
        this->ratio = ratio;
        this->MAX_ITER = MAX_ITER;
        this->k = k;
    }
    void run(string input_file, string output_file)
    {
        this->read_input(input_file);
        cerr << "Input read" << endl;
        this->initial_partition();
        cerr << "Done initial partition" << endl;
        this->compute_initial_distribution();
        cerr << "Computed the initial distribution" << endl;
        this->compute_initial_gain();
        cerr << "Computed the initial gain" << endl;
        int i = 0;
        while (i <= this->MAX_ITER && this->iterate())
        {
            cerr << "Done the " << i << "-th iteration" << endl;
            ++i;
        }
        this->write_output(output_file);
    }
    void read_input(string filename)
    {
        ifstream fin(filename);
        //
        fin >> this->number_of_cells >> this->number_of_nets;

        this->gain = new int[this->number_of_cells];
        this->distribution[0] = new int[this->number_of_nets];
        this->distribution[1] = new int[this->number_of_nets];
        this->nets_to_cells = new set<int>[this->number_of_nets];
        this->cells_to_nets = new set<int>[this->number_of_cells];
        this->seed = time(0);
        srand(this->seed);
        /* */
        this->free_cells = new bool[this->number_of_cells];
        for (int i = 0; i < this->number_of_cells; ++i)
            this->free_cells[i] = false;
        this->count_free_cells = this->number_of_cells;

        for (int i = 0; i < this->number_of_nets; ++i)
        {
            // for net # i
            int cell;
            while (fin >> cell)
            {
                // for cell # cell
                if (cell == -1)
                {
                    break;
                }
                else
                {
                    this->nets_to_cells[i].insert(cell);
                    this->cells_to_nets[cell].insert(i);
                }
                // cerr << cell << " ";
            }
            // cerr << endl;
        }

        this->pmax = -1;
        for (int i = 0; i < this->number_of_cells; ++i)
        {
            cerr << i << " " << this->cells_to_nets[i].size() << endl;
            // cerr << this->pmax < this->cells_to_nets[i].size() << endl;
            int value = this->cells_to_nets[i].size();
            if ((this->pmax) < (value))
            {
                // cerr << "****" << endl;
                this->pmax = this->cells_to_nets[i].size();
            }
        }
        // cerr << this->number_of_cells << "\t" << this->pmax << endl;
        this->bucket_structure_a = new BucketListStructure(this->number_of_cells, this->pmax);
        this->bucket_structure_b = new BucketListStructure(this->number_of_cells, this->pmax);
        // cerr << "Read input" << endl;
    }
    void write_output(string filename)
    {
        ofstream fout(filename);
        for (int i = 0; i < this->number_of_cells; ++i)
        {
            fout << i << "\t" << (this->partition[i] ? 0 : 1) << endl;
        }
        fout.close();
    }
    ~FMAlgorithm()
    {
        delete[] this->nets_to_cells;
    }
};

#endif