#include <sys/timeb.h>
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>

using namespace std;

#define random(x) (rand()%x)
inline int belonging(int x, int y) { return x / 3 * 3 + y / 3 + 1; } // ��������[x][y]���ڵڼ����Ź���
inline int position(int x, int y) { return x * 9 + y; } // ��������[x][y]���ĸ�λ��
const int M = 81;
const int N = 10;
const int all = 1000000;
static int final_sudoku[all][M]; // ���ɵ������վ�
static int** answer_sudoku; // ���ɵ�������Ϸ����������
static int cur_num;

class sudoku {
private:
	int map[M]; // 9x9����
	int hasAnswer; // �Ƿ��н�
	int number; // Ҫ�������������
	//int cur_num; // ��ǰ��������
	bool valid_col[N][N], valid_row[N][N], valid_grid[N][N];
	int **limit; // ��������ÿ�����ӵĶ�����ÿһ������������ 
	int **limit_backup[M]; // ���ڻ���
	int game[M];
	bool isEmpty[M];
	int num_0;
	int id;
public:
	sudoku() {
		limit = new int* [M];
		for (int j = 0;j < M;j++) {
			limit[j] = new int[N];
			memset(limit[j], 0, N * 4);
		}
		for (int i = 0;i < M;i++) {
			limit_backup[i] = new int* [M];
			for (int j = 0;j < M;j++) {
				limit_backup[i][j] = new int[N];
				memset(limit_backup[i][j], 0, N * 4);
			}
		}
		memset(map, 0, sizeof(map));
		hasAnswer = 0;
		number = 0;
		//cur_num = 0;
		memset(valid_row, false, sizeof(valid_row));
		memset(valid_col, false, sizeof(valid_col));
		memset(valid_grid, false, sizeof(valid_grid));
		memset(game, 0, sizeof(game));
		memset(isEmpty, false, sizeof(isEmpty));
		num_0 = 0;
		id = 0;
		//memset(limit, 0, M * N * 4);
		//memset(limit_backup, 0, M * M * N * 4);
		//final_sudoku = new int* [all];
		answer_sudoku = new int* [5000];
		/*for (int i = 0;i < all;i++) {
			final_sudoku[i] = new int[M];
			memset(final_sudoku[i], 0, M * 4);
		}*/
		for (int i = 0;i < 5000;i++) {
			answer_sudoku[i] = new int[M];
			memset(answer_sudoku[i], 0, M * 4);
		}
	}
	ofstream out; // Ҫд���ļ�
	//----------------��������------------------
	bool check(int x, int y, int z); // �ж�����[x][y]λ���ܷ�Ϊz
	// void transform(int a[M]); // �ѵ�ǰ������������ת����ת����һ��Ϊ1..9������
	// bool isEquivalent(int a[M], int b[M]); // �ж�����a������b�ǲ��ǵ�Ч
	bool deleteElement(int pos, int r); // ��������еĲ���_����һ������Ԫ�ص�����
	bool modifyElement(int pos, int r); // ��������еĲ���_���һ������Ԫ��
	void dfs_generate(int k, int type); // ���������Ļ���
	void dfs_solve(int k, int type); // ��������Ļ���
	void dfs_dig(int k, int total); //������Ԫ�ص���������total���յĻ���
	int freedom(int a[M]); // �������a�����ɶ�
	void write_file(int* s); // д�ļ�

	double getRandData(int min, int max); // �������0-1�еĸ�����
	int get_solution(int a[M]); // ������������Ľ�ĸ���

	void generate_final(int number, int type); // ����number�������վ֣����浽�ļ���

	//-----------------����-------------------
	// ����number��������Ϸ������Ľ���洢��result��
	void generate(int number, int** result);
	// ����number��ָ���Ѷȵȼ���������Ϸ������Ľ���洢��result��
	void generate(int number, int mode, int** result);
	// ����number���հ����½�Ϊlower���Ͻ�Ϊupper��������Ϸ������Ľ���洢��result��
	void generate(int number, int lower, int upper, int** result);
	// ����number����Ψһ��������Ϸ������Ľ���洢��result��
	void generate(int number, bool unique, int **result);

	// ����numer��������0�ĸ�����[from, ran+from-1]�У����ɶ���[dow, upd]��
	void generate(int number, int from, int ran, int dow, int upd, bool unique, int** result);
	// ����һ�����飬��num_0��0���������ɶ�
	int generate_single(int num_0, int id);
	//-----------------���--------------------
	bool solve_problem(fstream& in);
	// ���������������Ŀproblem������������չ��Ϊһά���飩������һ��boolֵ��ʾ�Ƿ��н⡣����н⣬��һ�����н�洢��solution�У�ͬ����ʾΪ���������һά����չ����
	// bool solve(int problem[M], int solution[M]);
};