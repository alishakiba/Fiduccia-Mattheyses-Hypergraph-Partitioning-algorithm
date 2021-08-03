# Sample data files:
## sampleHierarchy.txt
This file contains all categories and products. Products are leaves in the tree.
    6
    5
    0 1 -1
    1 2 3 4 -1
    2 5 6 -1
    3 7 8 9 -1
    4 10 11 -1
    5 12 13 14 15 -1

The first line is the number of categories.

The second line is the height of the tree.

The next lines are as follows:

`category_id [members]+ -1`

Note that each line ends with -1 for the categories and the entries are space separated.

## sampleTransactions.txt
This file contains all transactions.
    5
    1 6 10 15 14 12 -1
    2 10 12 14 11 15 -1
    3 7 -1
    4 10 12 14 5 -1
    5 9 10 8 13 -1

The first line is the number of transactions.
Then, on each line afterwards, we have

`factor_id [products]+ -1`

Note that each line ends with -1 for the products and the entries are space separated.


# Sample output

`make`
` ./main.exe data/sampleHierarchy.txt data/sampleTransactions.txt 1.5`

gives


    5: 12 13 14 15
    4: 10 11
    3: 7 8 9
    2: 5 6
    1: 2 3 4
    0: 1
    6 7 8 9 10 11 12 13 14 15
    Printing factor 1: 6 10 12 14 15
    The underlying data for factor 1:
    0: 1
    12:
    1: 2 4
    14:
    5: 15 14 12
    10:
    4: 10
    6:
    15:
    2: 6 5

    Printing factor 2: 10 11 12 14 15
    The underlying data for factor 2:
    0: 1
    2: 5
    15:
    11:
    1: 4 2
    14:
    12:
    5: 12 14 15
    10:
    4: 10 11

    Printing factor 3: 7
    The underlying data for factor 3:
    0: 1
    1: 3
    7:
    3: 7

    Printing factor 4: 5 10 12 14
    The underlying data for factor 4:
    0: 1
    2: 5 5
    1: 4 2
    14:
    12:
    5:
    10:
    4: 10

    Printing factor 5: 8 9 10 13
    The underlying data for factor 5:
    2: 5
    1: 3 4 2
    0: 1
    13:
    5: 13
    8:
    10:
    4: 10
    9:
    3: 9 8

    Printing factor 1: 6 10 12 14 15
    Printing factor 1: 6 10 12 14 15
    0
    Printing factor 1: 6 10 12 14 15
    Printing factor 2: 10 11 12 14 15
    0.408531
    Printing factor 1: 6 10 12 14 15
    Printing factor 3: 7
    0.604265
    Printing factor 1: 6 10 12 14 15
    Printing factor 4: 5 10 12 14
    0.424171
    Printing factor 1: 6 10 12 14 15
    Printing factor 5: 8 9 10 13
    0.533175

    Printing factor 2: 10 11 12 14 15
    Printing factor 1: 6 10 12 14 15
    0.408531
    Printing factor 2: 10 11 12 14 15
    Printing factor 2: 10 11 12 14 15
    0
    Printing factor 2: 10 11 12 14 15
    Printing factor 3: 7
    0.668246
    Printing factor 2: 10 11 12 14 15
    Printing factor 4: 5 10 12 14
    0.341232
    Printing factor 2: 10 11 12 14 15
    Printing factor 5: 8 9 10 13
    0.464455

    Printing factor 3: 7
    Printing factor 1: 6 10 12 14 15
    0.604265
    Printing factor 3: 7
    Printing factor 2: 10 11 12 14 15
    0.668246
    Printing factor 3: 7
    Printing factor 3: 7
    0
    Printing factor 3: 7
    Printing factor 4: 5 10 12 14
    0.540284
    Printing factor 3: 7
    Printing factor 5: 8 9 10 13
    0.630332

    Printing factor 4: 5 10 12 14
    Printing factor 1: 6 10 12 14 15
    0.424171
    Printing factor 4: 5 10 12 14
    Printing factor 2: 10 11 12 14 15
    0.341232
    Printing factor 4: 5 10 12 14
    Printing factor 3: 7
    0.540284
    Printing factor 4: 5 10 12 14
    Printing factor 4: 5 10 12 14
    0
    Printing factor 4: 5 10 12 14
    Printing factor 5: 8 9 10 13
    0.421801

    Printing factor 5: 8 9 10 13
    Printing factor 1: 6 10 12 14 15
    0.533175
    Printing factor 5: 8 9 10 13
    Printing factor 2: 10 11 12 14 15
    0.464455
    Printing factor 5: 8 9 10 13
    Printing factor 3: 7
    0.630332
    Printing factor 5: 8 9 10 13
    Printing factor 4: 5 10 12 14
    0.421801
    Printing factor 5: 8 9 10 13
    Printing factor 5: 8 9 10 13
    0

    3 nearest factors to 1 are 1 2 4
    3 nearest factors to 2 are 2 4 1
    3 nearest factors to 3 are 3 4 1
    3 nearest factors to 4 are 4 2 5
    3 nearest factors to 5 are 5 4 2
    Now, you can easily do the rest Mr. Iranmanesh.

