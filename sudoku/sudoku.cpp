#include <iostream>
#include<fstream>
#include <stdlib.h>
#include <time.h> 
#include <algorithm>
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
int firstLine[9] = { 1,2,3,4,5,6,7,8,9 };//第一行
int shift[8] = { 3, 6, 1, 4, 7, 2, 5, 8 };//第2-9行所需移位
int cv1[3] = { 3, 4, 5 };
int cv2[3] = { 6, 7, 8 };
int s1=1, s2=1;

void swap(int &a, int &b) {
	int temp = a;
	a = b;
	b = temp;
}
//清空输出文件夹
void init() {
	memcpy(sudoku_map[0], firstLine, sizeof(firstLine));
	ofstream clear;
	clear.open("D://Software Project//sudoku//test.txt");
	clear << "";
	clear.close();
	out.open("D://Software Project//sudoku//test.txt", ios::app);
}

void exchange_456(int s1)     //中间4，5，6行的变换
{
	if (s1 == 1) { cv1[0] = 3; cv1[1] = 4; cv1[2] = 5; }
	if (s1 == 2) { cv1[0] = 3; cv1[1] = 5; cv1[2] = 4; }
	if (s1 == 3) { cv1[0] = 4; cv1[1] = 3; cv1[2] = 5; }
	if (s1 == 4) { cv1[0] = 4; cv1[1] = 5; cv1[2] = 3; }
	if (s1 == 5) { cv1[0] = 5; cv1[1] = 3; cv1[2] = 4; }
	if (s1 == 6) { cv1[0] = 5; cv1[1] = 4; cv1[2] = 3; }
}

void exchange_789(int s2)     //最后7，8，9行的变换
{
	if (s2 == 1) { cv2[0] = 6; cv2[1] = 7; cv2[2] = 8; }
	if (s2 == 2) { cv2[0] = 6; cv2[1] = 8; cv2[2] = 7; }
	if (s2 == 3) { cv2[0] = 7; cv2[1] = 6; cv2[2] = 8; }
	if (s2 == 4) { cv2[0] = 7; cv2[1] = 8; cv2[2] = 6; }
	if (s2 == 5) { cv2[0] = 8; cv2[1] = 6; cv2[2] = 7; }
	if (s2 == 6) { cv2[0] = 8; cv2[1] = 7; cv2[2] = 6; }
}

//辅助函数 终盘->文件
void out_to_txt() {
	exchange_456(s1);
	exchange_789(s2); //判断当前两个参数表示哪两种顺序
	//输出数独
	for (int i = 0; i < 9; i++)
	{
		int k = i;
		if (k >= 3 && k <= 5) k = cv1[k - 3];
		else if (k > 5) k = cv2[k - 6];
		for (int j = 0; j <= 8; j++) {
			int kt = j * 2;
			out << sudoku_map[k][j];
			out << " ";
		}
		out << endl;
	}
	out << endl;
}

void produce()  //第一行已知，通过变换得到完整的数独终局
{
	for (int i = 1; i < 9; i++)
		for (int j = 0; j < 9; j++)
			sudoku_map[i][j] = sudoku_map[0][(j + shift[i - 1]) % 9];
	//第i行是第一行左移shift[i]得到的  即sudo[i][j] = sudo[0][j右移shift[i]]
	//j右移后可能超出9，所以要模9取余
}






// 生成指定数目的数独终局,出于性能考虑，使用矩阵转化法而非随机法
void generate_sudoku() {
	int cnt_number = 0;
	while (cnt_number++ < c) {
		if (s2 == 6) {
			if (s1 == 6) {
				next_permutation(firstLine,firstLine+9);
				memcpy(sudoku_map[0], firstLine, sizeof(firstLine));
				s1 = 1;
			}
			else s1++;
			s2 = 1;
		}
		produce();
		out_to_txt();
		s2++;
		if(cnt_number%1000 == 0)
			cout << cnt_number << endl;
	}
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
