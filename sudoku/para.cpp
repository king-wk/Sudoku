#include"para.h"
void para::setPara(int argc, char**argv) {
	this->argc = argc;
	this->argv = argv;
}

// 计算s字符串对应的值，如果不是数字串，就返回false
bool para::calc(char* s) {
	number = 0;
	for (int i = 0; s[i]; ++i) {
		if (s[i] < '0' || s[i] > '9')
			return false;
		number = number * 10 + s[i] - '0';
	}
	return true;
}
int para::getRightCode() {
	return this->rightCode;
}

int para::getNumber() {
	return this->number;
}

string para::getWrongCode() {
	return this->WrongCode;
}

bool para::check() {
	if (this->argc < 3 || strlen(argv[1])!=2) {
		this->WrongCode = "Insufficient argument";
		return false;
	}
	if (strcmp(argv[1], "-c") == 0) {
		if (argc > 3) {
			this->WrongCode = "Excessive arguments of -c";
			return false;
		}
		if (!calc(argv[2])) {
			this->WrongCode = "Use a number after -c";
			return false;
		}
		if (number < 1 || number > 1000000) {
			this->WrongCode = "-c number should be in [1,1000000]";
			return false;
		}
		this->rightCode = GEN;
		return true;
	}
	if (strcmp(argv[1], "-s") == 0) {
		if (argc > 3) {
			this->WrongCode = "Excessive arguments of -s";
			return false;
		}
		fstream in;
		in.open(argv[2], ios::in);
		if (!in) {
			this->WrongCode = "-s file doesn't exist";
			return false;
		}
		in.close();
		this->rightCode = SOLVE;
		return true;
	}
	if (strcmp(argv[1], "-n") == 0) {
		if (!calc(argv[2])) {
			this->WrongCode = "Use a number after -n";
			return false;
		}
		if (number < 1 || number > 10000) {
			this->WrongCode = "-n number should be in [1,10000]";
			return false;
		}// exe -n xx
		if (argc == 3) {
			this->rightCode = GAME;
			return true;
		}
		if (argc == 4) {
			// exe -n xx -u
			if (strcmp(argv[3], "-u") != 0) {
				this->WrongCode = "Parameter error";
				return false;
			}
			this->rightCode = UNI;
			return true;
		}
		// exe -n xx -m xx    exe -n xx -r xx~xx
		if (argc == 5) {
			if (strcmp(argv[3], "-m") == 0) {
				int len = strlen(argv[4]);
				int mode = argv[4][0] - '0';
				if (len > 1 || (mode != 1 && mode != 2 && mode != 3)) {
					this->WrongCode = "-m number should be 1 or 2 or 3";
					return false;
				}
				this->mode = mode;
				this->rightCode = RANK;
				return true;
			}
			if (strcmp(argv[3], "-r") == 0) {
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
						this->WrongCode = "-r r1~r2 must be int";
						return false;
					}
				}
				cout << "lower=" << lower << "upper=" << upper << endl;
				if (flag != 1) {
					this->WrongCode = "-r r1~r2, there must be a ~";
					return false;
				}
				if (lower < 1 || upper > 64 || lower > upper) {
					this->WrongCode = "-r r1~r2 should be in [1, 64] and r1<=r2";
					return false;
				}
				this->upper = upper;
				this->lower = lower;
				this->rightCode = DIG;
				return true;
			}
			this->WrongCode = "Parameter error";
			return false;
		}
		this->WrongCode = "Wrong number of parameters";
		return false;
	}
// 没有-n的时候
	this->WrongCode = "must be with -n before -m or -r";
	return false;
}
	
	
