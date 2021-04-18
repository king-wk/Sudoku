#include"sudoku.h"
#include"para.h"



int main(int argc, char* argv[]) {
	srand((unsigned int)time(0));
	sudoku s;
	para check;
	check.setPara(argc, argv);
	try {
		if (!check.check())
			throw exception(check.getWrongCode().c_str());
		else {
			if (check.getRightCode() == GEN) {
				s.out.open("generate_sudoku.txt", ios::out);
				// 生成数独终局
				s.generate_final(check.getNumber(), 1);
				cout << "The sudoku final generated is in the generate_sudoku.txt\n"
					<< "Have a check!\n";
			}
			else if (check.getRightCode() == SOLVE) {
				fstream in;
				in.open(argv[2], ios::in);
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
			else {
				s.out.open("problem_sudoku.txt", ios::out);
				if (check.getRightCode() == GAME) {
					s.generate(check.getNumber(), answer_sudoku);	
				}
				else if (check.getRightCode() == UNI) {
					s.generate(check.getNumber(),1,answer_sudoku);
				}
				else if (check.getRightCode() == RANK) {
					s.generate(check.getNumber(), (int)check.getMode(), answer_sudoku);
				}
				else if (check.getRightCode() == DIG) {
					s.generate(check.getNumber(), check.getLower(), check.getUpper(), answer_sudoku);
				}
				for (int i = 0; i < check.getNumber(); i++) {
					s.write_file(answer_sudoku[i]);
				}
				cout << "The sudoku problems generated is in the problem_sudoku.txt\n"
					<< "Have a check!\n";
			}
		}
	}
	catch (const std::exception& e)
	{
		puts(e.what());
	}
	s.out.close();
	return 0;
}