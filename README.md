# Implementation of FM Algorithm

This is my attempt to implement the *FM-algorithm* based on the original paper

    C. M. Fiduccia and R. M. Mattheyses. 1982. A linear-time heuristic for improving network partitions. In <i>Proceedings of the 19th Design Automation Conference</i> (<i>DAC '82</i>). IEEE Press, 175–181.

# Input Format

    #of_cells #of_nets
    cells in each net -1

# Building the project

For running the tests with `GoogleTest`, run the following command:

   bazel test --test_output=all //test:hello_test

For building the full project, run the following command:

   bazel build //main:FM_algorithm --compilation_mode=dbg  --compiler=mingw-gcc

Running the algorithm:

   .\bazel-bin\main\FM_algorithm.exe .\SampleInput.txt .\SampleOutput.txt [# of iterations] [balance ratio] [k] verbose