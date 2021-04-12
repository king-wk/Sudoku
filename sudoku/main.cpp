#include"sudoku.h"

int number;

// 计算s字符串对应的值，如果不是数字串，就返回false
bool calc(char* s) {
	number = 0;
	for (int i = 0; s[i]; ++i) {
		if (s[i] < '0' || s[i] > '9')
			return false;
		number = number * 10 + s[i] - '0';
	}
	return true;
}

void input_rule() {
	cout << "Valid input\n"
		<< "Usage:\n"
		<< "      sudoku.exe -c count --> generate given count sudoku finals. \n"
		<< "      sudoku.exe -s path --> Read sudoku from file in the given path, and solve them.\n"
		<< "      sudoku.exe -n number --> Generate given number sudoku games\n"
		<< "      sudoku.exe -n number -m 1/2/3 --> Generate given number sudoku games with 1/2/3 level difficulty.\n"
		<< "      sudoku.exe -n number -r lower~upper --> Generate given number sudoku games with given blank number.\n"
		<< "      sudoku.exe -n number -u --> Generating given number sudoku games with only one solution.\n";
}

int main(int argc, char* argv[]) {
	srand((unsigned int)time(0));
	sudoku s;
	try {
		if (argc < 3 || strlen(argv[1]) != 2) {
			throw exception("Insufficient argument");
		}
		if (strcmp(argv[1], "-c") == 0) {
			if (argc > 3) {
				throw exception("Excessive arguments of -c");
			}
			if (!calc(argv[2])) {
				throw exception("Use a number after -c");
			}
			if (number < 1 || number > 1000000) {
				throw exception("-c number should be in [1,1000000]");
			}
			s.out.open("generate_sudoku.txt", ios::out);
			// 生成数独终局
			s.generate_final(number, 1);
			cout << "The sudoku final generated is in the generate_sudoku.txt\n"
				<< "Have a check!\n";
		}
		else if (strcmp(argv[1], "-s") == 0) {
			if (argc > 3) {
				throw exception("Excessive arguments of -s");
			}
			fstream in;
			in.open(argv[2], ios::in);
			if (!in) {
				throw exception("-s file doesn't exist");
			}
			s.out.open("sudoku.txt", ios::out);
			// 求解数独
			if (s.solve_problem(in)) {
				cout << "The answer you need is in the sudoku.txt\n"
					<< "Have a check!\n";
			}
			else {
				throw exception("Wrong sudoku problem");
			}
			in.close();
		}
		else if (strcmp(argv[1], "-n") == 0) {
			if (!calc(argv[2])) {
				throw exception("Use a number after -n");
			}
			if (number < 1 || number > 10000) {
				throw exception("-n number should be in [1,10000]");
			}// exe -n xx
			if (argc == 3) {
				s.out.open("problem_sudoku.txt", ios::out);
				s.generate(number, answer_sudoku);
				for (int i = 0;i < number;i++) {
					s.write_file(answer_sudoku[i]);
				}
				cout << "The sudoku problems generated is in the problem_sudoku.txt\n"
					<< "Have a check!\n";
			}
			else if (argc == 4) {
				// exe -n xx -u
				if (strcmp(argv[3], "-u") == 0) {
					s.out.open("problem_sudoku.txt", ios::out);
					s.generate(number, (bool)true, answer_sudoku);
					for (int i = 0;i < number;i++) {
						s.write_file(answer_sudoku[i]);
					}
					cout << "The sudoku problems generated is in the problem_sudoku.txt\n"
						<< "Have a check!\n";
				}
				else {
					throw exception("Parameter error");
				}
			}
			// exe -n xx -m xx    exe -n xx -r xx~xx
			else if (argc == 5) {
				if (strcmp(argv[3], "-m") == 0) {
					int len = strlen(argv[4]);
					int mode = argv[4][0] - '0';
					if (len > 1 || (mode != 1 && mode != 2 && mode != 3)) {
						throw exception("-m number should be 1 or 2 or 3");
					}
					s.out.open("problem_sudoku.txt", ios::out);
					s.generate(number, (int)mode, answer_sudoku);
					for (int i = 0;i < number;i++) {
						s.write_file(answer_sudoku[i]);
					}
					cout << "The sudoku problems generated is in the problem_sudoku.txt\n"
						<< "Have a check!\n";
				}
				else if (strcmp(argv[3], "-r") == 0) {
					int flag = 0, lower = 0, upper = 0;
					int len = strlen(argv[4]);
					for (int i = 0; i < len; i++) {
						if (argv[4][i] == '~') {
							flag = 1;
						}
						else if (argv[4][i] >= '0' && argv[4][i] <= '9') {
							if (!flag) {
								lower = lower * 10 + argv[4][i] - '0';
							}
							else {
								upper = upper * 10 + argv[4][i] - '0';
							}
						}
						else {
							throw exception("-r r1~r2 must be int");
						}
					}
					cout << "lower=" << lower << "upper=" << upper << endl;
					if (flag != 1) {
						throw exception("-r r1~r2, there must be a ~");
					}
					else if (lower < 1 || upper > 64 || lower > upper) {
						throw exception("-r r1~r2 should be in [1, 64] and r1<=r2");
					}
					s.out.open("problem_sudoku.txt", ios::out);
					s.generate(number, lower, upper, answer_sudoku);
					for (int i = 0;i < number;i++) {
						s.write_file(answer_sudoku[i]);
					}
					cout << "The sudoku problems generated is in the problem_sudoku.txt\n"
						<< "Have a check!\n";
				}
				else {
					throw exception("Parameter error");
				}
			}
			else {
				throw exception("Wrong number of parameters");
			}
		}
		else { // 没有-n的时候
			throw exception("must be with -n before -m or -r");
		}
	}
	catch (const std::exception& e)
	{
		puts(e.what());
	}
	s.out.close();
	return 0;
}