  // Copyright (c) 2009 Google Inc. All rights reserved.

// Author: Wen Jingjing & Xiao Zhongyao

// Date:2021-04-15

#include "build/sudoku.h"

bool sudoku::check(int x, int y, int z) {
    return (!valid_row[x][z] && !valid_col[y][z] && !valid_grid[belonging(x, y)][z]);// NOLINT
}

void sudoku::generate_final(int n, int type) {
    memset(valid_grid, false, sizeof(valid_grid));
    memset(valid_col, false, sizeof(valid_col));
    memset(valid_row, false, sizeof(valid_row));
    cur_num = 0;
    number = n;
    hasAnswer = false;
    dfs_generate(0, type);
}

void sudoku::write_file(int *s) {
    char temp[300];
    int len = 0;
    for (int i = 0; i < 81; i++) {
        if (s[i] == 0) {
            temp[len++] = '$';
        } else if (s[i] > 0 && s[i] <= 9) {
            temp[len++] = s[i] + '0';
        }
        temp[len++] = (((i + 1) % 9) == 0) ? '\n' : ' ';
    }
    temp[len++] = '\n';
    temp[len] = '\0';
    cout << temp << endl;
    out << temp;
}

void sudoku::dfs_generate(int k, int type) {
    if (hasAnswer) {
        return;
    }
    if (k > 80) {
        if (type) {
            write_file(map);
        }
        memcpy(final_sudoku[cur_num], map, sizeof(map));
        cur_num++;
        if (cur_num >= number) {
            hasAnswer = 1;
        }
        return;
    } else {
        int x = k / 9;
        int y = k % 9;
        int p = belonging(x, y);
        for (int z = 1; z <= 9; z++) {
            if (check(x, y, z) && !hasAnswer) {
                map[k] = z;
                valid_row[x][z] = valid_col[y][z] = valid_grid[p][z] = true;
                dfs_generate(k + 1, type);
                valid_row[x][z] = valid_col[y][z] = valid_grid[p][z] = false;
            }
        }
    }
}

// Add a Sudoku element restriction to
// judge whether the pos can't be filled with r
bool sudoku::deleteElement(int pos, int r) {
    int i;
    if (limit[pos][r] == 1) {
        return true;
    }
    limit[pos][r] = 1;  // Increase the limit, do not allow to fill in r
    // It means that there is no value to fill in in this lattice,
    // all values are limited
    if (++limit[pos][0] == 9) {
        return false;
    }
    // There is only one value left to fill in
    if (limit[pos][0] == 8) {
        for (i = 1; i <= 9; ++i) {
            if (limit[pos][i] == 0) {
                break;
            }
        }
        if (!modifyElement(pos, i)) {
            return false;
        }
    }
    return true;
}

// Fill in an element to increase the limit
bool sudoku::modifyElement(int pos, int r) {
    int x = pos / 9;
    int y = pos % 9;
    int h;
    map[pos] = r;
    for (int i = 1; i <= 9; i++) {
        limit[pos][i] = 1;
    }
    limit[pos][0] = 8;
    limit[pos][r] = 0;  // You can only fill in r
    // Add restrictions on other grids of peers
    for (int i = 0; i < 9; i++) {
        h = position(x, i);
        // If r can also be filled in the place other than po,
        // it will be an error
        if (h != pos && !deleteElement(h, r)) {
            return false;
        }
    }
    // Add restrictions on other cells in the same column
    for (int i = 0; i < 9; i++) {
        h = position(i, y);
        if (h != pos && !deleteElement(h, r)) {
            return false;
        }
    }
    // Increase the limit of other grids with nine palace lattices
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            h = (3 * (x / 3) + i) * 9 + (3 * (y / 3) + j);
            if (h != pos && !deleteElement(h, r)) {
                return false;
            }
        }
    }
    return true;
}

void sudoku::dfs_solve(int k, int type) {
    int mx = -1, mxi;
    for (int i = 0; i < 81; i++) {
        // Found the empty space with the most restrictions
        // and not filled in yet
        if (limit[i][0] < 8 && limit[i][0] > mx) {
            mx = limit[i][0];
            mxi = i;
        }
    }
    // It's all filled in
    if (mx == -1) {
        if (type) {
            write_file(map);
        }
        hasAnswer++;
        return;
    }
    memcpy(limit_backup[k], limit, M * N * 4);
    for (int i = 1; i <= 9; i++) {
        if (limit[mxi][i] == 0) {
            if (modifyElement(mxi, i)) {
                dfs_solve(k + 1, 1);
            }
            memcpy(limit, limit_backup[k], M * N * 4);
        }
        if (hasAnswer >= 2) {
            return;
        }
    }
}

bool sudoku::solve_problem(fstream &in) {
    char s[20];
    int total = 0;
    while (in.getline(s, 20)) {
        for (int i = 0; i < strlen(s); i++) {
            if (s[i] == '$') {
                map[total++] = 0;
            } else {
                if (s[i] <= '0' || s[i] > '9')
                    continue;
                map[total++] = s[i] - '0';
            }
            if (total == 81) {
                for (int j = 0; j < 81; j++) {
                    if (map[j] > 0) {
                        // If the original elements are contradictory,
                        // stop solving
                        if (!modifyElement(j, map[j])) {
                            return false;
                        }
                    }
                }
                dfs_solve(1, 1);
                total = 0;
                hasAnswer = 0;
                for (int j = 0; j < M; j++) {
                    memset(limit[j], 0, N * 4);
                }
            }
        }
    }
    return true;
}

// Calculating the degree of freedom of Sudoku
int sudoku::freedom(int a[M]) {
    int res = 0, x, y, h;
    for (int node = 0; node < 81; node++) {
        if (a[node] != 0)
            continue;
        x = node / 9;
        y = node % 9;
        for (int q = 0; q < 9; q++) {
            if (q != y && a[x * 9 + q] == 0) {
                res++;
            }
        }
        for (int p = 0; p < 9; p++) {
            if (p != x && a[p * 9 + y] == 0) {
                res++;
            }
        }
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                h = (3 * (x / 3) + i) * 9 + (3 * (y / 3) + j);
                if (h != node && a[h] == 0) {
                    res++;
                }
            }
        }
    }
    return res;
}

int sudoku::getFree() {
    return this->free;
}

double sudoku::getRandData(int min, int max) {
    return (rand() % 101) / 100;// NOLINT
}

// From the backtracking of digging holes in Sudoku filled with elements,
// we have dug total + 1
void sudoku::dfs_dig(int k, int total) {
    if (k > 80) {
        return;
    }
    double t = (getRandData(0, 1) + getRandData(0, 1)) / 2;
    // When the number of remaining lattices is larger than
    // the number of remaining empty zeros,
    // we can randomly determine whether empty zeros are needed
    if ((total == num_0 || t >= 0.5) && (81 - k >= num_0 - total + 1)) {
        // Fill in this grid
        game[k] = final_sudoku[id][k];
        dfs_dig(k + 1, total);
    } else {
        // Empty the grid
        game[k] = 0;
        dfs_dig(k + 1, total + 1);
    }
}

int sudoku::generate_single(int num_0_t, int id_t) {
    id = id_t;
    num_0 = num_0_t;
    memset(game, 0, sizeof(game));
    dfs_dig(0, 0);
    int free = freedom(game);
    return free;
}

int sudoku::get_solution(int a[M]) {
    hasAnswer = 0;
    memcpy(map, a, sizeof(map));
    memset(a, 0, sizeof(a));
    for (int i = 0; i < 81; i++) {
        if (a[i] > 0) {
            modifyElement(i, a[i]);
        }
    }
    dfs_solve(1, 0);
    return hasAnswer;
}

// Generate number Sudoku, the number of 0 is in [from, ran + from - 1],
// the degree of freedom is in [dow, upd]
void sudoku::generate(int number, int from, int ran, int dow, int upd, bool unique, int** result) {  // NOLINT
    int first, fre, cnt_f = 0, id_t = 0, uni, times = 0;
    while (cnt_f < number) {
        first = random(ran) + from;
        fre = generate_single(first, id_t);
        if (fre >= dow && fre <= upd) {
            if (unique) {
                uni = get_solution(game);
                if (uni > 1) {
                    times++;
                    if (times > 100) {
                        id_t++;
                        times = 0;
                    }
                    continue;
                }
            }
            this->free = fre;
            memcpy(result[cnt_f++], game, sizeof(game));
        }
        id_t++;
        times = 0;
    }
}

void sudoku::generate(int number, int **result) {
    generate_final(50000, 0);
    generate(number, 16, 17, 0, 650, false, result);
}

void sudoku::generate(int number, int mode, int **result) {
    generate_final(50000, 0);
    switch (mode) {
    case 1:
        generate(number, 16, 17, 0, 650, false, result);
        break;
    case 2:
        generate(number, 32, 17, 651, 999, false, result);
        break;
    case 3:
        generate(number, 48, 17, 1000, 1944, false, result);
        break;
    }
}

void sudoku::generate(int number, int lower, int upper, int **result) {
    generate_final(50000, 0);
    generate(number, lower, upper - lower + 1, 0, 1944, false, result);
}

void sudoku::generate(int number, bool unique, int **result) {
    generate_final(50000, 0);
    generate(number, 16, 17, 0, 650, true, result);
}
