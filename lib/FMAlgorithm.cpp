#include "FMAlgorithm.hpp"

fm::FMAlgorithm::FMAlgorithm(double ratio, int k, int max_iter, bool verbose)
    : ratio {ratio}, k {k}, MAX_ITER {max_iter}, verbose {verbose} 
{
    // nothing to do
    if (verbose) {
        std::cout << "In FM Constructor:" << std::endl;
        std::cout << "\tratio: " << this->ratio << std::endl;
        std::cout << "\tk: " << this->k << std::endl;
        std::cout << "\tmax_iter" << this->MAX_ITER;
        std::cout << "\tverbose: " << this->verbose << std::endl;
    }
}

fm::FMAlgorithm::~FMAlgorithm() {
    delete this->bucketA;
    delete this->bucketB;
    delete this->cells_to_nets;
    delete this->cells_to_nets;

    delete[] this->partition;
    delete[] this->free;
    delete[] gain;

    delete[] this->distribution[0];
    delete[] this->distribution[1];
}

bool fm::FMAlgorithm::is_balance_criterion_satisfied(char from) {
    int size = ((from == 'a') ? this->size_A - 1 : this->numOfCells - this->size_A - 1);
    if (this->numOfCells * this->ratio - this->k - 1 <= size
            && size <= this->numOfCells * this->ratio + this->k + 1) {
            // we can move from A without violating the balance criterion
            if (verbose) {
                std::cout << "\t\tBalance criterion is satisfied. from " << from << ": "
                        << this->numOfCells * this->ratio - this->k - 1 
                        << " <= " << size << " <= "
                        << this->numOfCells * this->ratio + this->k + 1 
                        << std::endl;
            }
            return true;
    }
    else {
        // the balancing criterion is violated if we move a cell
        if (verbose) {
            std::cout << "\t\tBalance criterion is violated. from " << from << ": "
                    << this->numOfCells * this->ratio - this->k - 1 
                    << " !<= " << size << " !<= "
                    << this->numOfCells * this->ratio + this->k + 1 
                    << " ***** numOfCells: " << this->numOfCells 
                    << " ratio: " << this->ratio 
                    << " k: " << this->k
                    << " size_A: " << size_A
                    << std::endl;
        }
        return false;
    }
}

void fm::FMAlgorithm::initial_partition(const std::set<int>* partition_A) {
    if (verbose) {
        std::cout << "In initial_partition:" << std::endl;
        std::cout << "bucket_A: " << std::endl;
        this->bucketA->print();
        std::cout << "bucket_B: " << std::endl;
        this->bucketB->print();
    }
    // initialize the partition
    if (partition_A == nullptr) {
        // taking care of the balancing criterion
        int max_size_of_A = this->numOfCells * this->ratio + this->k;
        
        // randomly partition the data
        for (int i = 0; i < this->numOfCells; i++) {
            if (this->dis(this->gen) % 2 == 0 && this->size_A < max_size_of_A) {
                this->partition[i] = 'a';
                this->size_A++;
            }
            else {
                this->partition[i] = 'b';
            }
        }
        // check if the balancing criterion is satisfied
        if (this->size_A != max_size_of_A) {
            // move some cells from B to A to make the balance criterion satisfied
            int num_of_cells_to_move = max_size_of_A - this->size_A;
            int current_b_cell_to_move = 0;
            for (int i = 0; i < num_of_cells_to_move; i++) {
                while (this->partition[current_b_cell_to_move] != 'b') {
                    current_b_cell_to_move++;
                }
                this->partition[current_b_cell_to_move] = 'a';
            }
        }
    }
    else {
        // if the partition is given, we use it
        for (int i = 0; i < this->numOfCells; i++) {
            this->partition[i] = 'b';
        }
        for (auto cell : *partition_A) {
            this->partition[cell] = 'a';
            this->size_A++;
        }
    }
}

void fm::FMAlgorithm::initial_distribution() {
    for(int i = 0; i < this->numOfNets; ++i) {
        int A = 0;
        int B = 0;
        for(auto cell: this->nets_to_cells[i]) {
            if (this->partition[cell] == 'a') {
                A++;
            }
            else {
                B++;
            }
        }
        this->distribution[0][i] = A;
        this->distribution[1][i] = B;
    }
}

void fm::FMAlgorithm::initial_gain() {
    for(int i =0; i < this->numOfCells; ++i) {
        // computing gain for cell i
        this->gain[i] = 0;
        for(auto net: cells_to_nets[i]) {
            // for each net which cell i belongs to 
            if (this->partition[i] == 'a') {
                // cell i belongs to partition A,
                // so F=A and T=B according to the 
                // terminology of the paper
                if (this->distribution[0][i] == 1) {
                    this->gain[i]++;
                }
                if (this->distribution[1][i] == 0) {
                    this->gain[i]--;
                }
            }
            else {
                // cell i belongs to partition B,
                // so F=B and T=A according to the 
                // terminology of the paper
                if (this->distribution[1][i] == 1) {
                    this->gain[i]++;
                }
                if (this->distribution[0][i] == 0) {
                    this->gain[i]--;
                }
            }
        }
        // adding cell i to the BucketListStructure for
        // corresponding partition
        if (this->partition[i] == 'a') {
            // cell i belongs to partition A
            this->bucketA->add_cell(i, this->gain[i]);
        }
        else {
            // cell i belongs to partition B
            this->bucketB->add_cell(i, this->gain[i]);
        }
    }
}

void fm::FMAlgorithm::move_base_cell(int cell) {
    int T, F;
    if (this->partition[cell] == 'a') {
        // this cell belongs to partition A
        // so, F=A and T=B
        T = 1;
        F = 0;
    }
    else {
        // this cell belongs to partition B
        // so, F=B and T=A
        T = 0;
        F = 1;
    }
    // lock the base cell
    this->free[cell] = false;
    this->countOfFreeCells--;
    // remove the base cell from the BucketListStructure
    if (this->partition[cell] == 'a') {
        // cell belongs to partition A
        this->bucketA->remove_cell(cell);
    }
    else {
        // cell belongs to partition B
        this->bucketB->remove_cell(cell);
    }
    // change the base cell's partition
    this->partition[cell] = (this->partition[cell] == 'a') ? 'b' : 'a';
    // if (this->verbose) {
    //     std::cout << "\tSuccessfully removed base cell " 
    //             << cell << " to " 
    //             << this->partition[cell] << std::endl;
    // }
    // update the distribution
    for(auto net: this->cells_to_nets[cell]) {
        // check critical nets
        if (distribution[T][net] == 0) {
            // increment gains of all free cells on net 
            for(auto free_cell: nets_to_cells[net]) {
                if (this->free[free_cell]) {
                    // it is a free cell
                    this->gain[free_cell]++;
                    if (this->partition[free_cell] == 'a') {
                        // it belongs to partition A
                        this->bucketA->add_cell(free_cell, this->gain[free_cell]);
                    }
                    else {
                        // it belongs to partition B
                        this->bucketB->add_cell(free_cell, this->gain[free_cell]);
                    }
                }
            }
        }
        else if (distribution[T][net] == 1) {
            // decrement gain of the only free cell on partition T
            for(auto free_cell: nets_to_cells[net]) {
                if (this->free[free_cell]) {
                    if (this->partition[free_cell] == 'a' && T == 0) {
                        // it belongs to partition A
                        this->gain[free_cell]--;
                        this->bucketA->add_cell(free_cell, this->gain[free_cell]);
                    }
                    else if (this->partition[free_cell] == 'b' && T == 1) {
                        // it belongs to partition B
                        this->gain[free_cell]--;
                        this->bucketB->add_cell(free_cell, this->gain[free_cell]);
                    }
                    else {
                        // no need to update gain since the change in the partition does not affect the gain for this cell
                    }
                }
            }
        }
        // change the net distribution to reflect the move
        this->distribution[F][net]--;
        this->distribution[T][net]++;
        // check critical nets after the move
        if (distribution[F][net] == 0) {
            // decrement gains of all free cells on net 
            for(auto free_cell: nets_to_cells[net]) {
                if (this->free[free_cell]) {
                    // it is a free cell
                    this->gain[free_cell]--;
                    if (this->partition[free_cell] == 'a') {
                        // it belongs to partition A
                        this->bucketA->add_cell(free_cell, this->gain[free_cell]);
                    }
                    else {
                        // it belongs to partition B
                        this->bucketB->add_cell(free_cell, this->gain[free_cell]);
                    }
                }
            }
        }
        else if (distribution[F][net] == 1) {
            // decrement gain of the only free cell on partition F
            for(auto free_cell: nets_to_cells[net]) {
                if (this->free[free_cell]) {
                    if (this->partition[free_cell] == 'a' && F == 0) {
                        // it belongs to partition A
                        this->gain[free_cell]++;
                        this->bucketA->add_cell(free_cell, this->gain[free_cell]);
                    }
                    else if (this->partition[free_cell] == 'b' && F == 1) {
                        // it belongs to partition B
                        this->gain[free_cell]++;
                        this->bucketB->add_cell(free_cell, this->gain[free_cell]);
                    }
                }
            }
        }
    }

    if (F == 0) {
        this->size_A--;
    }
    else {
        this->size_A++;
    }
}

bool fm::FMAlgorithm::single_move() {
    auto t1 = std::chrono::high_resolution_clock::now();
    if (this->countOfFreeCells == 0) {
        return false;
    }
    // identifying the base cell
    int max_gain_a = this->bucketA->getMaxGain();
    int max_gain_b = this->bucketB->getMaxGain();
    int base_cell = -1;
    if (!this->bucketA->isEmpty() && max_gain_a >= max_gain_b && this->is_balance_criterion_satisfied('a')) {
        // we can move from A to B without violating the balance criterion
        base_cell = this->bucketA->getMaxGainCellID();
        assert(base_cell != -1);
        assert(this->free[base_cell]);
    }
    else if (!this->bucketB->isEmpty() && max_gain_a <= max_gain_b && this->is_balance_criterion_satisfied('b')) {
        // we can move from B to A without violating the balance criterion
        base_cell = this->bucketB->getMaxGainCellID();
        assert(base_cell != -1);
        assert(this->free[base_cell]);
    }
    else if (!this->bucketA->isEmpty() && this->is_balance_criterion_satisfied('a')) {
        // we can move from A to B without violating the balance criterion
        base_cell = this->bucketA->getMaxGainCellID();
        assert(base_cell != -1);
        assert(this->free[base_cell]);
    }
    else if (!this->bucketB->isEmpty() && this->is_balance_criterion_satisfied('b')) {
        // we can move from B to A without violating the balance criterion
        base_cell = this->bucketB->getMaxGainCellID();
        assert(base_cell != -1);
        assert(this->free[base_cell]);
    }
    else {
        // no further moves are possible
        if (this->verbose) {
            std::cout << "\tCannot move base cell: " << base_cell
            << " balance: " << this->is_balance_criterion_satisfied('a') 
            << " gain: " << this->gain[base_cell] 
            << " gain_a: " << max_gain_a
            << " gain_b: " << max_gain_b
            << std::endl;
        }
        return false;
    }
    if (this->verbose) {
        std::cout << "\tMoving base cell: " << base_cell;
    }
    this->move_base_cell(base_cell);
    if (this->verbose) {
        auto t2 = std::chrono::high_resolution_clock::now();
        std::cout << " finished after " 
            << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
            << " ms." << std::endl;
    }
    return true;
}

void fm::FMAlgorithm::read_input(std::string input_filename) {
    auto t1 = std::chrono::high_resolution_clock::now();
    if (this->verbose) {
        std::cout << "Reading input file ";
    }
    std::ifstream fin(input_filename);
    if (!fin.is_open()) {
        std::cerr << "Error: Cannot open input file " << input_filename << std::endl;
        throw std::runtime_error("Error: Cannot open input file " + input_filename);
    }
    //
    fin >> this->numOfCells >> this->numOfNets;
    if (this->verbose) {
        std::cout << "numOfCells: " << this->numOfCells << " numOfNets: " << this->numOfNets << std::endl;
    }
    this->gain = new int[this->numOfCells];
    this->distribution[0] = new int[this->numOfNets];
    this->distribution[1] = new int[this->numOfNets];
    this->nets_to_cells = new std::set<int>[this->numOfNets];
    this->cells_to_nets = new std::set<int>[this->numOfCells];
    this->free = new bool[this->numOfCells];
    this->partition = new char[this->numOfCells];
    /* */
    for(int i = 0; i < this->numOfCells; ++i) 
        this->free[i] = true;
    this->countOfFreeCells = this->numOfCells;
    
    for (int i = 0; i < this->numOfNets; ++i) {
        // for net # i
        int cell;
        while(fin >> cell) {
            // for cell # cell
            if (cell == -1) {
                break;
            }
            else {
                this->nets_to_cells[i].insert(cell);
                this->cells_to_nets[cell].insert(i);
            }
        }
    }

    this->pmax = 0;
    for(int i = 0; i < this->numOfCells; ++i) {
        if (this->pmax < this->cells_to_nets[i].size()) {
            this->pmax = this->cells_to_nets[i].size();
        }
    }

    this->bucketA = new fm::BucketList(this->pmax, this->numOfCells);
    this->bucketB = new fm::BucketList(this->pmax, this->numOfCells);

    if (this->verbose) {
        auto t2 = std::chrono::high_resolution_clock::now();
        std::cout << " finished after " 
            << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
            << " ms." << std::endl;
    }
}
void fm::FMAlgorithm::write_output(std::string filename) {
    auto t1 = std::chrono::high_resolution_clock::now();
    if (this->verbose) {
        std::cout << "Writing output file ";
    }
    std::ofstream fout(filename);
    if (!fout.is_open()) {
        std::cerr << "Error: Cannot open output file " << filename << std::endl;
        throw std::runtime_error("Error: Cannot open output file " + filename);
    }
    for(int i = 0; i < this->numOfCells; ++i) {
        fout << i << "\t" << (this->partition[i] == 'a' ? 0 : 1) << std::endl;
    }
    fout.close();
    if (this->verbose) {
        auto t2 = std::chrono::high_resolution_clock::now();
        std::cout << " finished after " 
            << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
            << " ms." << std::endl;
    }
}

bool fm::FMAlgorithm::iterate() {
    static int iter = 0;
    auto t1 = std::chrono::high_resolution_clock::now();
    if (this->verbose) {
        std::cout << "Iteration # " << iter << " started." << std::endl;
    }
    while (this->single_move()) {
        // do nothing    
    }
    if (this->verbose) {
        auto t2 = std::chrono::high_resolution_clock::now();
        std::cout << "Iteration # " << iter << " finished after " 
            << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
            << " ms." << std::endl;
    }
    iter++;
    if (this->countOfFreeCells != 0) {
        // we cannot move a cell due to the balance criterion
        // so, this is true even for the next iterations and 
        // we need to stop the algorithm
        return false;
    }

    // all cells are considered now, so let's re-initialize the values for free cells
    for(int i = 0; i < this->numOfCells; ++i) 
        this->free[i] = true;
    this->countOfFreeCells = this->numOfCells;
    
    return true;
}

void fm::FMAlgorithm::run(std::string input_filename, std::string output_filename) {
    this->read_input(input_filename);
    this->initial_partition();
    this->initial_distribution();
    this->initial_gain();

    if (verbose) {
        std::cout << "Before the first iteration:" 
        << " size_A: " << this->size_A 
        << " size_B: " << this->numOfCells - this->size_A
        << std::endl;
        std::cout << "bucket_A: " << std::endl;
        this->bucketA->print();
        std::cout << "bucket_B: " << std::endl;
        this->bucketB->print();
    }

    int i = 0;
    while (i <= this->MAX_ITER && this->iterate()) {
            ++i;
    }

    this->write_output(output_filename);
}

