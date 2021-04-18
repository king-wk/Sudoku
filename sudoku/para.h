#include"common.h"
enum rights { GEN = 0, SOLVE, DIG,UNI,RANK,GAME };
class para {
private:
	int argc;
	char **argv;
	string WrongCode;
	int rightCode;
	int number;
	int mode;
	int lower;
	int upper;
public:
	void setPara(int argc, char**argv);
	string getWrongCode();
	int getRightCode();
	bool check();
	bool calc(char* s);
	int getNumber();
	int getMode() { return this->mode; }
	int getLower() { return this->lower; }
	int getUpper() { return this->upper; }
};
