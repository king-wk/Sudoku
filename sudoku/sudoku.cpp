#include <iostream>
using namespace std;

int sudoku_map[9][9]; // 棋盘
int c = 0; // 生成数独终局的个数
char gamepath[64];
int level = 1; // 难度等级
int minblank = 0; // 挖空个数
int maxblank = 0;
int number = 0; // 生成游戏个数
bool unique = false; // 生成解是否唯一

// 生成指定数目的数独终局
void generate_sudoku() {
	int cnt_number = 0; // 已经生成的数独数目
	while (cnt_number < c) {

	}
}

// 生成指定数目的数独游戏
void generate_sudoku_problem() {

}

void solve_single_sudoku() {

}

// 求解指定路径的数独题目
void solve_sudoku(string gamepath) {

}

void print_error_info() {
	cout << "Illegal input\n"
		<< "Usage:\n"
		<< "      sudoku.exe -c count --> generate given count sudoku finals. \n"
		<< "      sudoku.exe -s path --> Read sudoku from file in the given path, and solve them.\n"
		<< "      sudoku.exe -n number --> Generate given number sudoku games\n"
		<< "      sudoku.exe -n number -m 1/2/3 --> Generate given number sudoku games with 1/2/3 level difficulty.\n"
		<< "      sudoku.exe -n number -r blank_number --> Generate given number sudoku games with given blank number.\n"
		<< "      sudoku.exe -n number -u --> Generating given number sudoku games with only one solution.\n";
}

int main(int argc, char** argv)
{
	if (argc == 3 && (!strcmp(argv[1], "-c") || !strcmp(argv[1], "-s") || !strcmp(argv[1], "-n"))) {
		// 求解指定文件的数独问题，将解答保存到sudoku.txt
		if (!strcmp(argv[1], "-s")) {
			strcpy(gamepath, argv[2]);
			solve_sudoku(gamepath);
			cout << "The answer you need is in the sudoku.txt\n"
				<< "Have a check!\n";
			return 0;
		}
		else {
			int len = strlen(argv[2]);
			for (int i = 0; i < len; i++)
			{
				if (argv[2][i] > '9' || argv[2][i] < '0')
				{
					print_error_info();
					return 0;
				}
			}
			int n = 0;
			for (int i = 0; i < len; i++)
			{
				n = n * 10 + argv[2][i] - '0';
			}
			// 生成count个数独终局存到文件generate_sudoku.txt中
			if (!strcmp(argv[1], "-c")) {
				c = n;
				generate_sudoku();
				cout << "The sudoku final generated is in the generate_sudoku.txt\n"
					<< "Have a check!\n";
				return 0;
			}
			// 生成指定数量的数独问题
			if (!strcmp(argv[1], "-n")) {
				number = n;
				generate_sudoku_problem();
				cout << "The sudoku problems generated is in the sudoku_problem.txt\n"
					<< "Have a check\n";
				return 0;
			}
		}
	}
	// 生成指定数量且只有唯一解的数独问题
	else if (argc == 4 && !strcmp(argv[1], "-n") && !strcmp(argv[3], "-u")) {
		int len = strlen(argv[2]);
		for (int i = 0; i < len; i++)
		{
			if (argv[2][i] > '9' || argv[2][i] < '0')
			{
				print_error_info();
				return 0;
			}
		}
		int n = 0;
		for (int i = 0; i < len; i++)
		{
			n = n * 10 + argv[2][i] - '0';
		}
		if (!strcmp(argv[1], "-n")) {
			number = n;
			generate_sudoku_problem();
			cout << "The sudoku problems generated is in the sudoku_problem.txt\n"
				<< "Have a check\n";
			return 0;
		}
	}
	else if (argc == 5 && !strcmp(argv[1], "-n") && (!strcmp(argv[3], "-m") || !strcmp(argv[3], "-r"))) {
		int len = strlen(argv[2]);
		for (int i = 0; i < len; i++)
		{
			if (argv[2][i] > '9' || argv[2][i] < '0')
			{
				print_error_info();
				return 0;
			}
		}
		int n = 0;
		for (int i = 0; i < len; i++)
		{
			n = n * 10 + argv[2][i] - '0';
		}
		// 生成指定数量且指定难度的数独问题
		if (!strcmp(argv[3], "-m")) {
			int len = strlen(argv[4]);
			if (len == 1) {
				level = argv[4][0] - '0';
				number = n;
				generate_sudoku_problem();
				cout << "The sudoku problems generated is in the sudoku_problem.txt\n"
					<< "Have a check\n";
				return 0;
			}
			else {
				print_error_info();
			}
		}
		// 生成指定数量且指定空白数量的数独问题
		else {
			int minblank = 0, maxblank = 0;
			int len = strlen(argv[4]);
			for (int i = 0; i < len; i++)
			{
				if ((argv[4][i] > '9' || argv[4][i] < '0') && (argv[4][i] != '~' || i == 0 || i == len - 1))
				{
					print_error_info();
					return 0;
				}
			}
			int flag = 0;
			for (int i = 0; i < len; i++)
			{
				if (argv[4][i] == '~') {
					flag = 1;
				}
				if (flag) {
					maxblank= maxblank * 10 + argv[4][i] - '0';
				}
				else {
					minblank = minblank * 10 + argv[4][i] - '0';
				}
			}
			number = n;
			generate_sudoku_problem();
			cout << "The sudoku problems generated is in the sudoku_problem.txt\n"
				<< "Have a check\n";
			return 0;
		}
	}
	else {
		print_error_info();
	}
	return 0;
}
