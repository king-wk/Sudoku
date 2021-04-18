#include"common.h"

using namespace std;

#define random(x) (rand()%x)
inline int belonging(int x, int y) { return x / 3 * 3 + y / 3 + 1; } // 计算数独[x][y]属于第几个九宫格
inline int position(int x, int y) { return x * 9 + y; } // 计算数独[x][y]在哪个位置
const int M = 81;
const int N = 10;
const int all = 1000000;
static int final_sudoku[all][M]; // 生成的数独终局
static int** answer_sudoku; // 生成的数独游戏或者数独答案
static int cur_num;

class sudoku {
private:
	int map[M]; // 9x9数独
	int hasAnswer; // 是否有解
	int number; // 要输出的数独个数
	//int cur_num; // 当前数独个数
	bool valid_col[N][N], valid_row[N][N], valid_grid[N][N];
	int **limit; // 保存数独每个格子的对于填每一个数的限制数 
	int **limit_backup[M]; // 用于回溯
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
	ofstream out; // 要写的文件
	//----------------辅助函数------------------
	bool check(int x, int y, int z); // 判读数独[x][y]位置能否为z
	// void transform(int a[M]); // 把当前数独进行数字转换，转换成一行为1..9的数独
	// bool isEquivalent(int a[M], int b[M]); // 判断数独a与数独b是不是等效
	bool deleteElement(int pos, int r); // 求解数独中的操作_增加一个数独元素的限制
	bool modifyElement(int pos, int r); // 求解数独中的操作_填充一个数独元素
	void dfs_generate(int k, int type); // 生成数独的回溯
	void dfs_solve(int k, int type); // 求解数独的回溯
	void dfs_dig(int k, int total); //从填满元素的数独中挖total个空的回溯
	int freedom(int a[M]); // 求解数独a的自由度
	void write_file(int* s); // 写文件

	double getRandData(int min, int max); // 随机返回0-1中的浮点数
	int get_solution(int a[M]); // 返回数独数组的解的个数

	void generate_final(int number, int type); // 生成number个数独终局，保存到文件中

	//-----------------生成-------------------
	// 生成number个数独游戏，计算的结果存储在result中
	void generate(int number, int** result);
	// 生成number个指定难度等级的数独游戏，计算的结果存储在result中
	void generate(int number, int mode, int** result);
	// 生成number个空白数下界为lower，上界为upper的数独游戏，计算的结果存储在result中
	void generate(int number, int lower, int upper, int** result);
	// 生成number个解唯一的数独游戏，计算的结果存储在result中
	void generate(int number, bool unique, int **result);

	// 生成numer个数独，0的个数在[from, ran+from-1]中，自由度在[dow, upd]中
	void generate(int number, int from, int ran, int dow, int upd, bool unique, int** result);
	// 生成一个数组，有num_0个0，返回自由度
	int generate_single(int num_0, int id);
	//-----------------求解--------------------
	bool solve_problem(fstream& in);
	// 对于输入的数独题目problem（数独矩阵按行展开为一维数组），返回一个bool值表示是否有解。如果有解，则将一个可行解存储在solution中（同样表示为数独矩阵的一维数组展开）
	// bool solve(int problem[M], int solution[M]);
};