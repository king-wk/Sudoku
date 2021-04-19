#include "sudoku.h"
bool sudoku::check(int x, int y, int z) {
	// 只需要判断第x行，第y列还没有填上z，[x][y]所属九宫格还没有填上z即可
	return (!valid_row[x][z] && !valid_col[y][z] && !valid_grid[belonging(x, y)][z]);
}

// 生成到final_sudoku里面
void sudoku::generate_final(int n, int type) {
	memset(valid_grid, false, sizeof(valid_grid));
	memset(valid_col, false, sizeof(valid_col));
	memset(valid_row, false, sizeof(valid_row));
	cur_num = 0;
	number = n;
	hasAnswer = false;
	dfs_generate(0, type);
}
// 写数独
void sudoku::write_file(int* s) {
	char temp[300];
	int len = 0;
	for (int i = 0;i < 81;i++) {
		if (s[i] == 0) {
			temp[len++] = '$';
		}
		else if (s[i] > 0 && s[i] <= 9) {
			temp[len++] = s[i] + '0';
		}
		temp[len++] = (((i + 1) % 9) == 0) ? '\n' : ' ';
	}
	temp[len++] = '\n';
	temp[len] = '\0';
	cout << temp << endl;
	out << temp;
	//cout << "写完了" << endl;
}

void sudoku::dfs_generate(int k, int type) {
	if (hasAnswer) {
		//cout << "cur_num" << cur_num << endl;
		return;
	}
	// k表示已经填完的空
	if (k > 80) {
		if (type) {
			//cout << "write..." << endl;
			write_file(map);
			//cout << "write finish" << endl;
		}
		// cout << cur_num << " " << number << endl;
		memcpy(final_sudoku[cur_num], map, sizeof(map)); // 存储答案
		cur_num++;
		if (cur_num >= number) {
			// cout << cur_num << " " << number << endl;
			hasAnswer = 1;
		}
		return;
	}
	int x = k / 9;
	int y = k % 9;
	int p = belonging(x, y);
	for (int z = 1;z <= 9;z++) {
		if (check(x, y, z) && !hasAnswer) {
			map[k] = z;
			valid_row[x][z] = valid_col[y][z] = valid_grid[p][z] = true;
			dfs_generate(k + 1, type);
			valid_row[x][z] = valid_col[y][z] = valid_grid[p][z] = false;
		}
	}
}

// 增加一个数独元素的限制，判断pos处是不是不能填r
bool sudoku::deleteElement(int pos, int r) {
	//cout << "delete " << pos << " r=" << r << endl;
	int i;
	//cout << "limit" << limit[pos][r] << endl;
	if (limit[pos][r] == 1) { // 已经存在不能填r的限制，没问题
		return true;
	}
	//cout << "a" << endl;
	limit[pos][r] = 1; // 增加限制，不让填r
	if (++limit[pos][0] == 9) { // 代表这个格子没有值可以填了，所有的值都限制了
		return false;
	}
	//cout << "b" << endl;
	if (limit[pos][0] == 8) { // 还剩下一个值可以填
		for (i = 1; i <= 9; ++i) {
			if (limit[pos][i] == 0) {  // 找到是谁可以填
				break;
			}
		}
		if (!modifyElement(pos, i)) { // 填它，如果不行，完蛋了
			return false;
		}
	}
	//cout << "c" << endl;
	return true;
}

// 填充一个元素，增加限制
bool sudoku::modifyElement(int pos, int r) {
	int x = pos / 9;  // 行号
	int y = pos % 9;  // 列号
	int h;
	map[pos] = r;
	for (int i = 1;i <= 9;i++) {
		limit[pos][i] = 1;
	}
	limit[pos][0] = 8;
	limit[pos][r] = 0;  // 只能填r
	for (int i = 0; i < 9; i++) {// 增加同行的其它格子的限制
		h = position(x, i);
		// 在不是pos的地方如果也可以填r，则错误
		if (h != pos && !deleteElement(h, r)) {
			return false;
		}
	}
	//cout << "row.." << endl;
	for (int i = 0; i < 9; i++) {// 增加同列的其它格子的限制
		h = position(i, y);
		if (h != pos && !deleteElement(h, r)) {
			return false;
		}
	}
	//cout << "col.." << endl;
	for (int i = 0; i < 3; i++) {// 增加同九宫格的其它格子的限制
		for (int j = 0; j < 3; j++) {
			h = (3 * (x / 3) + i) * 9 + (3 * (y / 3) + j);
			if (h != pos && !deleteElement(h, r)) {
				return false;
			}
		}
	}
	//cout << "grid.." << endl;
	return true;
}

void sudoku::dfs_solve(int k, int type) {
	int mx = -1, mxi;
	for (int i = 0;i < 81;i++) {
		// 找到限制最多且还没有填的那个空
		if (limit[i][0] < 8 && limit[i][0] > mx) {
			mx = limit[i][0];
			mxi = i;
		}
	}
	// 全部填完了
	if (mx == -1) {
		if (type) {
			write_file(map);
		}
		hasAnswer++;
		return;
	}
	memcpy(limit_backup[k], limit, M * N * 4);
	for (int i = 1;i <= 9;i++) {
		if (limit[mxi][i] == 0) { // 如果这个格子对填i没有限制，填一填，试一下啦
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

bool sudoku::solve_problem(fstream& in) {
	char s[20];
	int total = 0;
	//if (!in.is_open())return false;
	while (in.getline(s, 20)) {
		for (int i = 0;i < strlen(s);i++) {
			// cout << s[i];
			if (s[i] == '$') {
				map[total++] = 0;
			}
			else {
				if (s[i] <= '0' || s[i] > '9')
					continue;
				map[total++] = s[i] - '0';
			}
			if (total == 81) {
				for (int j = 0;j < 81;j++) {
					if (map[j] > 0) {
						//cout << "modifyed.." << endl;
						if (!modifyElement(j, map[j])) {// 如果原有元素矛盾，停止求解
							return false;
						}
						//cout << j << endl;
					}
				}
				//cout << "start solve.." << endl;
				dfs_solve(1, 1);
				total = 0;
				hasAnswer = 0;
				for (int j = 0;j < M;j++) {
					memset(limit[j], 0, N * 4);
				}
			}
		}
	}
	return true;
}

// 计算数独的自由度
int sudoku::freedom(int a[M]) {
	int res = 0, x, y, h;
	for (int node = 0;node < 81;node++) {
		if (a[node] != 0)
			continue;
		x = node / 9;
		y = node % 9;
		for (int q = 0;q < 9;q++) {
			if (q != y && a[x * 9 + q] == 0) {
				res++;
			}
		}
		for (int p = 0;p < 9;p++) {
			if (p != x && a[p * 9 + y] == 0) {
				res++;
			}
		}
		for (int i = 0;i < 3;i++) {
			for (int j = 0;j < 3;j++) {
				h = (3 * (x / 3) + i) * 9 + (3 * (y / 3) + j);
				if (h != node && a[h] == 0) {
					res++;
				}
			}
		}
	}
	return res;
	//this->free = res;
}

int sudoku::getFree() {
	return this->free;
}

double sudoku::getRandData(int min, int max) {
	return (double)(rand() % 101) / 100;
}

// 从填满元素的数独中挖空的回溯，已经挖了total+1个
void sudoku::dfs_dig(int k, int total) {
	if (k > 80) {
		return;
	}
	double t = getRandData(0, 1);
	if ((total == num_0 || t >= 0.5) && (81 - k >= num_0 - total))// 当剩下的格子大于剩余需要空0的个数，可以随机确定是否需要空0
	{
		//cout << "total" << total << endl;
		//cout << "k" << k << endl;
		//cout << "num_0" << num_0 << endl;
		// 这个格子不空
		game[k] = final_sudoku[id][k];
		dfs_dig(k + 1, total);
	}
	else {
		// 这个格子空了
		game[k] = 0;
		//cout << "空了" << k << endl;
		dfs_dig(k + 1, total + 1);
	}
}

int sudoku::generate_single(int num_0_t, int id_t) {
	id = id_t;
	num_0 = num_0_t;
	memset(game, 0, sizeof(game));
	//cout << "start dig" << endl;
	dfs_dig(0, 0);
	int free = freedom(game);
	// cout << "free" << free << endl;
	return free;
}

int sudoku::get_solution(int a[M]) {
	hasAnswer = 0;
	memcpy(map, a, sizeof(map));
	memset(a, 0, sizeof(a));
	for (int i = 0;i < 81;i++) {
		if (a[i] > 0) {
			modifyElement(i, a[i]);
		}
	}
	dfs_solve(1, 0);
	return hasAnswer;
}

// 生成numer个数独，0的个数在[from, ran + from - 1]中，自由度在[dow, upd]中
void sudoku::generate(int number, int from, int ran, int dow, int upd, bool unique, int** result) {
	// cout << "生成。。。" << endl;
	int first, fre, cnt_f = 0, id_t = 0, uni, times = 0;
	while (cnt_f < number) {
		first = random(ran) + from;
		// cout << first << endl;
		fre = generate_single(first, id_t);
		if (fre >= dow && fre <= upd) {
			this->free = fre;
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
			memcpy(result[cnt_f++], game, sizeof(game));
		}
		id_t++;
		times = 0;
	}
}

void sudoku::generate(int number, int** result) {
	generate_final(50000, 0);
	generate(number, 16, 17, 0, 650, false, result);
}

void sudoku::generate(int number, int mode, int** result) {
	generate_final(50000, 0);
	switch (mode)
	{
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

void sudoku::generate(int number, int lower, int upper, int** result) {
	generate_final(50000, 0);
	generate(number, lower, upper - lower + 1, 0, 1944, false, result);
}

void sudoku::generate(int number, bool unique, int** result) {
	generate_final(50000, 0);
	generate(number, 16, 17, 0, 650, true, result);
}