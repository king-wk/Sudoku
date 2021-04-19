/**************************************************************************
 * Copyright (c) 2009 Google Inc. All rights reserved.
 * 
 * Author: Wen Jingjing & Xiao Zhongyao
 * 
 * Date:2021-04-15
**************************************************************************/
#ifndef D__CPP_NOTHING_SOFTWAREENGINEERING_BUILD_SUDOKU_H_
#define D__CPP_NOTHING_SOFTWAREENGINEERING_BUILD_SUDOKU_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

using namespace std;// NOLINT

#define random(x) (rand() % x)// NOLINT

// To calculate the number of ninth grid of Sudoku[x][y]
inline int belonging(int x, int y) {
    return x / 3 * 3 + y / 3 + 1;
}
// To calculate the position of Sudoku[x][y]
inline int position(int x, int y) {
    return x * 9 + y;
}
const int M = 81;
const int N = 10;
const int all = 1000000;
static int final_sudoku[all][M];
static int **answer_sudoku;         // Generated Sudoku game or Sudoku answer
static int cur_num;

class sudoku {
 private:
    int map[M];       // 9x9sudoku
    int hasAnswer;    // Is there a solution
    int number;       // Number of Sudoku to output
    bool valid_col[N][N], valid_row[N][N], valid_grid[N][N];
    // Save the limit number of each Sudoku grid to fill in each number
    int **limit;
    int **limit_backup[M];  // For backtracking
    int game[M];
    bool isEmpty[M];
    int num_0;
    int id;
    int free;  // freedom

 public:
    sudoku() {
        limit = new int *[M];// NOLINT
        for (int j = 0; j < M; j++) {
            limit[j] = new int[N];
            memset(limit[j], 0, N * 4);
        }
        for (int i = 0; i < M; i++) {
            limit_backup[i] = new int *[M];
            for (int j = 0; j < M; j++) {
                limit_backup[i][j] = new int[N];
                memset(limit_backup[i][j], 0, N * 4);
            }
        }
        memset(map, 0, sizeof(map));
        hasAnswer = 0;
        number = 0;
        memset(valid_row, false, sizeof(valid_row));
        memset(valid_col, false, sizeof(valid_col));
        memset(valid_grid, false, sizeof(valid_grid));
        memset(game, 0, sizeof(game));
        memset(isEmpty, false, sizeof(isEmpty));
        num_0 = 0;
        id = 0;
        answer_sudoku = new int *[5000];
        for (int i = 0; i < 5000; i++) {
            answer_sudoku[i] = new int[M];
            memset(answer_sudoku[i], 0, M * 4);
        }
    }
    ofstream out;  // File to write

    // ----------------auxiliary function------------------

    // Judge whether the position of Sudoku[x][y] can be z
    bool check(int x, int y, int z);
    // Solve the operation in Sudoku and add the restriction of a Sudoku element
    bool deleteElement(int pos, int r);
    // Solve the operation in Sudoku and fill in a Sudoku element
    bool modifyElement(int pos, int r);
    // Backtracking of generating Sudoku
    void dfs_generate(int k, int type);
    // Backtracking in solving Sudoku
    void dfs_solve(int k, int type);
    // Trace back the total blanks from the Sudoku filled with elements
    void dfs_dig(int k, int total);
    // Finding the degree of freedom of solving Sudoku a
    int freedom(int a[M]);
    // Writing files
    void write_file(int *s);

    // Random return of floating point numbers in 0-1
    double getRandData(int min, int max);
    // Returns the number of solutions to the Sudoku array
    int get_solution(int a[M]);
    // get freedom
    int getFree();

    // -----------------generate functions-------------------

    // Generate number Sudoku finale, save to file
    void generate_final(int number, int type);
    // Generate number Sudoku games, and store the calculated results in result
    void generate(int number, int **result);
    // Generate number Sudoku games with specified difficulty level,
    // and store the calculated results in result
    void generate(int number, int mode, int **result);
    // Generate a Sudoku game with number blank numbers,
    // lower bound and upper bound,
    // and store the calculated results in result
    void generate(int number, int lower, int upper, int **result);
    // Generate number Sudoku game with unique solution
    // and store the result in result
    void generate(int number, bool unique, int **result);

    // Generate number Sudoku, the number of 0 is in [from, ran + from-1],
    // the degree of freedom is in [dow, upd]
    void generate(int number, int from, int ran, int dow, int upd, bool unique, int **result);// NOLINT
    // Generate a Sudoku with num_ 0 zeros, return degrees of freedom
    int generate_single(int num_0, int id);

    // -----------------Solving Sudoku--------------------
    bool solve_problem(fstream &in);
};

#endif  // D__CPP_NOTHING_SOFTWAREENGINEERING_BUILD_SUDOKU_H_
