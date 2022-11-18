#include <iostream>
#include <vector>

/* Summary of tasks:
1. take in nxn matrix
2. print matrix
3. convert to REF
4. print matrix
5. convert to RREF
6. print matrix */

void printMatrix (std::vector<std::vector<double>> matrix, int x) {
    if (x == 0) {
        std::cout << "\nHere is your matrix:\n\n";
    }
    else if (x == 1) {
        std::cout << "\nHere is your matrix in row echelon form:\n\n";
    }
    else {
        std::cout << "\nHere is your matrix in reduced row echelon form:\n\n";
    }
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

std::vector<std::vector<double>> buildMatrix () {
    int nro, ncol;
    double in;
    std::vector<double> temp;
    std::vector<std::vector<double>> matrix;

    std::cout << "\nHow many rows? ";
    std::cin >> nro;
    std::cout << "How many columns? ";
    std::cin >> ncol;
    std::cout << "\n";

    for (int i = 0; i < nro; ++i) {
        temp.clear();
        for (int j = 0; j < ncol; ++j) {
            std::cout << "Enter a value for row " << i + 1 << ", column " << j + 1 << ": ";
            std::cin >> in;
            temp.push_back(in);
        }
        matrix.push_back(temp);
    }

    return matrix;
}

std::vector<std::vector<double>> swapRows (std::vector<std::vector<double>> matrix, int row1, int row2) {
    std::vector<std::vector<double>> temp;
    temp = matrix;

    matrix[row1] = temp[row2];
    matrix[row2] = temp[row1];

    return matrix;
}

/* Outline of ref:
- Number of steps must be AT MOST number of rows in matrix
- Each step must follow this order:
1. Check if there is a next row to operate on (opRo)
-- Row must at least tie in minimum column to have nonzero entry
-- Row must not have any zeros
2. Move opRo to top (except for previously operated rows)
-- Through swapRows function
3. Divide opRo by leading nonzero value
-- Go right to left to not change leading nonzero value itself
4. Subtract each row below by multiple of this row
-- If the row is all 0's, nothing bad will happen */

std::vector<std::vector<double>> ref (std::vector<std::vector<double>> matrix) {
    
    
    for (int i = 0; i < matrix.size(); ++i) { // for each step
        // first determine first column with nonzero entry for each row (if there is no nonzero entry, use placeholder of -1)
        int opRo = 0, row0 = 0, minCol = matrix[0].size();
        std::vector<int> cols;
        for (int j = i; j < matrix.size(); ++j) { // for each row (skipping previous opRo) // this "outputs" list of cols
            for (int k = 0; k < matrix[j].size(); ++k) { // scan from left to right
                if (matrix[j][k] != 0) { // when find a nonzero entry in a row
                    cols.push_back(k);
                    break;
                }
                if (k == matrix[j].size() - 1) { // when find no entry in a row
                    cols.push_back(-1);
                }
            }
        }

        for (int j = 0; j < cols.size(); ++j) { // checks if there is any row to operate on (by going through each entry of cols)
            if (cols[j] == -1) {
                ++row0;
            }
        }
        if (row0 == cols.size()) { // if all rows are empty do nothing
            return matrix;
        }
        for (int j = 0; j < cols.size(); ++j) { // determine opRo
            if (cols[j] < minCol && cols[j] != -1) {
                minCol = cols[j];
                opRo = j + i; // take into account the skipped rows from before
            }
        } // finally, we have opRo and a leading coefficient at column minCol

        // move opRo to top
        matrix = swapRows(matrix, i, opRo);
        opRo = i;

        // tiny function which divides row by leading coefficient (from right to left)
        for (int j = matrix[opRo].size() - 1; j >= minCol; --j) { // for each entry in opRo (right to left), divide by lead coef
            matrix[opRo][j] /= matrix[opRo][minCol];
        }

        // tiny function which subtracts each row below by the product of opRo and k = leading coef of row below
        for (int j = opRo + 1; j < matrix.size(); ++j) { // for each row below
            for (int k = matrix[j].size() - 1; k >= minCol; --k) {
                matrix[j][k] -= matrix[opRo][k] * matrix[j][minCol];
            }
        }
    }

    return matrix;
}

/* Outline of ref2rref
- Number of steps should be equal to total number of rows - number of empty rows - 1(as you dont do anything with top row)
-- Calculate bottom-most, nonzero row
1. Find first column with nonzero entry in opRo (equal to col)
2. Subtract every row above by the product of opRo * row[col]
-- If it is a zero above the leading coef of opRo, nothing will happen to the row */

std::vector<std::vector<double>> ref2rref (std::vector<std::vector<double>> matrix) {
    int maxRo = -1;
    // tiny function to find bottom-most, nonzero row
    for (int i = 0; i < matrix.size(); ++i) { // for each row in matrix (top to bottom)
        if (maxRo == -1) {
            for (int k = 0; k < matrix[i].size(); ++k) { // go from left to right to scan for nonzero entry
                if (matrix[i][k] != 0) {
                    if (i == matrix.size() - 1) { // if there are no empty rows
                        maxRo = i;
                    }
                    break;
                }
                if (k == matrix[i].size() - 1) { // what to do when find empty row
                    maxRo = i - 1;
                }
            }
        }
    }

    // actual steps where starting row is maxRo (and opRo = i)
    for (int i = maxRo; i > 0; --i) {
        int col;
        // find first column with nonzero entry
        for (int j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] != 0) {
                col = j;
                break;
            }
        }

        // subtract each row above by the product of opRo * row[col]
        for (int j = i - 1; j >= 0; --j) {
            for (int k = matrix[j].size(); k >= col; --k) {
                matrix[j][k] -= matrix[i][k] * matrix[j][col];
            }
        }
    }

    return matrix;
}

int main () {
    std::vector<std::vector<double>> matrix = buildMatrix();
    printMatrix(matrix, 0);
    matrix = ref(matrix);
    printMatrix(matrix, 1);
    matrix = ref2rref(matrix);
    printMatrix(matrix, 2);
    return 0;
}