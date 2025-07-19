#include <iostream>
#include <pthread.h>
#include <vector>
using namespace std;

#define N 9

int sudoku[N][N] = {
    {5,3,4,6,7,8,9,1,2},
    {6,7,2,1,9,5,3,4,8},
    {1,9,8,3,4,2,5,6,7},
    {8,5,9,7,6,1,4,2,3},
    {4,2,6,8,5,3,7,9,1},
    {7,1,3,9,2,4,8,5,6},
    {9,6,1,5,3,7,2,8,4},
    {2,8,7,4,1,9,6,3,5},
    {3,4,5,2,8,6,1,7,9}
};

bool rowValid[N] = {false};
bool colValid[N] = {false};
bool gridValid[N] = {false};

// Function to check a row
void* checkRow(void* param) {
    for (int i = 0; i < N; i++) {
        bool digits[N + 1] = {false};
        for (int j = 0; j < N; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || digits[num]) return nullptr;
            digits[num] = true;
        }
        rowValid[i] = true;
    }
    return nullptr;
}

// Function to check a column
void* checkColumn(void* param) {
    for (int j = 0; j < N; j++) {
        bool digits[N + 1] = {false};
        for (int i = 0; i < N; i++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || digits[num]) return nullptr;
            digits[num] = true;
        }
        colValid[j] = true;
    }
    return nullptr;
}

// Function to check a 3x3 grid
void* checkGrid(void* param) {
    int grid = *(int*)param;
    int startRow = (grid / 3) * 3;
    int startCol = (grid % 3) * 3;

    bool digits[N + 1] = {false};
    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || digits[num]) return nullptr;
            digits[num] = true;
        }
    }
    gridValid[grid] = true;
    return nullptr;
}

int main() {
    pthread_t rowThread, colThread, gridThreads[9];
    int gridIndices[9];

    pthread_create(&rowThread, nullptr, checkRow, nullptr);
    pthread_create(&colThread, nullptr, checkColumn, nullptr);

    for (int i = 0; i < 9; i++) {
        gridIndices[i] = i;
        pthread_create(&gridThreads[i], nullptr, checkGrid, (void*)&gridIndices[i]);
    }

    pthread_join(rowThread, nullptr);
    pthread_join(colThread, nullptr);

    for (int i = 0; i < 9; i++) {
        pthread_join(gridThreads[i], nullptr);
    }

    // Final result
    bool valid = true;
    for (int i = 0; i < 9; i++) {
        if (!rowValid[i] || !colValid[i] || !gridValid[i]) {
            valid = false;
            break;
        }
    }

    if (valid)
        cout << "Sudoku solution is valid!" << endl;
    else
        cout << "Sudoku solution is invalid!" << endl;

    return 0;
}