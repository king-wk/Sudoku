#include <iostream>
#include<fstream>
#include <stdlib.h>
#include <time.h> 
#pragma warning(disable:4996)
using namespace std;


int seed_map[4][9][9];
int sudoku_map[9][9]; // 棋盘
int c = 0; // 生成数独终局的个数
char gamepath[64];
int level = 1; // 难度等级
int minblank = 0; // 挖空个数
int maxblank = 0;
int number = 0; // 生成游戏个数
bool unique = false; // 生成解是否唯一
ofstream out;
struct makePair {
	int cv1;
	int cv2;
	int cv3;
	bool operator == (const makePair &rhs);
};
bool makePair::operator == (const makePair &rhs)
{
	return ((cv1 == rhs.cv1) && (cv2 == rhs.cv2) && (cv3 == rhs.cv3));
}
makePair pairs[18] = {
		{1,2,3},{1,3,2},{2,1,3},{2,3,1},{3,2,1},{3,1,2},
		{4,5,6},{4,6,5},{5,4,6},{5,6,4},{6,5,4},{6,4,5},
		{7,8,9},{7,9,8},{8,7,9},{8,9,7},{9,8,7},{9,7,8}
};

//初始化种子终盘并清空输出文件夹
void init() {
	ifstream in;
	in.open("D://Software Project//sudoku//seed_map.txt");
	for (int k = 0; k < 4; k++) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				in >> seed_map[k][i][j];
			}
		}
	}
	in.close();//读取完成之后关闭文件
	out.open("D://Software Project//sudoku//test.txt", ios::app);
}

//辅助函数 终盘->文件
void out_to_txt() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			out << sudoku_map[i][j]<<" ";
		}
		out << '\n';
	}
	out << "\n";
}

//辅助函数（转换法）1 数字交换
void exchange_number(int value1,int value2) {
	//int randNum = rand() % 4;
	memcpy(sudoku_map, seed_map[0], sizeof(seed_map[0]));
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (sudoku_map[i][j] == value1) {
				sudoku_map[i][j] = 0;
			}
			if (sudoku_map[i][j] == value2) {
				sudoku_map[i][j] = value1;
			}
		}
	}
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (sudoku_map[i][j] == 0) {
				sudoku_map[i][j] = value2;
			}
		}
	}
}

//辅助函数（转换法）2 行交换
void exchange_row(int row1, int row2) {
	//int randNum = rand() % 4;
	memcpy(sudoku_map, seed_map[0], sizeof(seed_map[0]));
	int temp[9];
	memcpy(temp, sudoku_map[row1], sizeof(sudoku_map[row1]));
	memcpy(sudoku_map[row1], sudoku_map[row2], sizeof(sudoku_map[row2]));
	memcpy(sudoku_map[row2], temp, sizeof(temp));
}

//辅助函数（转换法）3 列交换
void exchange_col(int col1, int col2) {
	//int randNum = rand() % 4;
	memcpy(sudoku_map, seed_map[0], sizeof(seed_map[0]));
	int temp;
	for (int i = 0; i < 9; i++) {
		temp = sudoku_map[i][col1];
		sudoku_map[i][col1] = sudoku_map[i][col2];
		sudoku_map[i][col2] = temp;
	}
}

void exchange(int v1, int v2, bool is_row) {
	if (is_row)exchange_row(v1, v2);
	else exchange_col(v1, v2);
}

void make_row_col(makePair pair1,makePair pair2,makePair pair3,bool is_row) {
	if (pair1 == pairs[1])exchange(1, 2, is_row);
	if (pair1 == pairs[2])exchange(0, 1, is_row);
	if (pair1 == pairs[3]) {
		exchange(0, 1, is_row);
		exchange(1, 2, is_row);
	}
	if (pair1 == pairs[4])exchange(0, 2, is_row);
	if (pair1 == pairs[5]) {
		exchange(0, 2, is_row);
		exchange(1, 2, is_row);
	}
	if (pair2 == pairs[7])exchange(4, 5, is_row);
	if (pair2 == pairs[8])exchange(3, 4, is_row);
	if (pair2 == pairs[9]) {
		exchange(3, 4, is_row);
		exchange(4, 5, is_row);
	}
	if (pair2 == pairs[10])exchange(3, 5, is_row);
	if (pair2 == pairs[11]) {
		exchange(3, 5, is_row);
		exchange(4, 5, is_row);
	}
	if (pair3 == pairs[13])exchange(7, 8, is_row);
	if (pair3 == pairs[14])exchange(6, 7, is_row);
	if (pair3 == pairs[15]){
		exchange(6, 7, is_row); 
		exchange(7, 8, is_row);
	}
	if (pair3 == pairs[16])exchange(6, 8, is_row);
	if (pair3 == pairs[17]) {
		exchange(6, 8, is_row);
		exchange(7, 8, is_row);
	}
}

// 生成指定数目的数独终局,出于性能考虑，使用矩阵转化法而非随机法
void generate_sudoku() {
	int cnt_number = 0; // 已经生成的数独数目
	int value1 = 1,value2 = 1;
	int temp1[9][9], temp2[9][9];
	/*for (value1 = 1; value1 <= 9; value1++)
	{
		for (value2 = value1; value2 <= 9 && cnt_number < c; value2++)
		{

			if (value2 == value1 && value2 != 1)continue;
			exchange_number(value1, value2);*/
	memcpy(sudoku_map, seed_map[0], sizeof(seed_map[0]));
			memcpy(temp1, sudoku_map, sizeof(sudoku_map));
			for (int row_cv1 = 0; row_cv1 < 2; row_cv1++) {
				for (int row_cv2 = 6; row_cv2 < 12; row_cv2++) {
					for (int row_cv3 = 12; row_cv3 < 18; row_cv3++) {
						//memcpy(sudoku_map, temp1, sizeof(temp1));
						make_row_col(pairs[row_cv1], pairs[row_cv2], pairs[row_cv3], true);
						//memcpy(temp2, sudoku_map, sizeof(sudoku_map));
						for (int col_cv1 = 0; col_cv1 < 6; col_cv1++) {
							for (int col_cv2 = 6; col_cv2 < 12; col_cv2++) {
								for (int col_cv3 = 12; col_cv3 < 18 && cnt_number < c; col_cv3++) {
									//memcpy(sudoku_map, temp2, sizeof(temp2));
									make_row_col(pairs[col_cv1], pairs[col_cv2], pairs[col_cv3], false);
									cnt_number++;
									out_to_txt();
								}
							}
						}
					}
				}
			}
		//}
		/*cout << cnt_number << endl;
	}*/
	out.close();
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
	srand((unsigned)time(NULL));
	init();
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
	system("pause");
	return 0;
}
