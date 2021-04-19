#include "pch.h"
#include "CppUnitTest.h"
#include"D:\Software Project\sudoku\sudoku.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SudokuUnitTest2
{
	TEST_CLASS(SudokuUnitTest2)
	{
	public:
		sudoku s;
		TEST_METHOD(TestGenCount)
		{
			srand((unsigned int)time(0));
			int number = rand() % 1000000 + 1;
			s.out.open("generate_sudoku.txt", ios::out);
			s.generate_final(number, 1);
			s.out.close();
			fstream in;
			in.open("generate_sudoku.txt", ios::in);
			char buffer[20];
			int line = 0;
			while (in.getline(buffer, 20)) {
				if (buffer[0] == '\0')continue;
				line++;
			}
			in.close();
			Assert::AreEqual(number * 9, line);
		}

		TEST_METHOD(TestGenRule)
		{
			srand((unsigned int)time(0));
			int number = rand() % 10 + 1;
			s.out.open("generate_sudoku.txt", ios::out);
			s.generate_final(number, 1);
			s.out.close();
			fstream in;
			in.open("generate_sudoku.txt", ios::in);
			Assert::AreEqual(s.solve_problem(in), true);
		}

		TEST_METHOD(TestGenGame)
		{
			srand((unsigned int)time(0));
			int lower = rand() % 20 + 1;
			int higher = rand() % 55 + 1;
			int num = rand() % 10 + 1;
			//TODO:≤‚ ‘ ˝¡ø
			s.out.open("problem_sudoku.txt", ios::out);
			s.generate(num, answer_sudoku);
			for (int i = 0; i < num; i++) {
				s.write_file(answer_sudoku[i]);
			}
			s.out.close();
			fstream in;
			in.open("problem_sudoku.txt", ios::in);
			char buffer[20];
			int line = 0;
			while (in.getline(buffer, 20)) {
				if (buffer[0] == '\0')continue;
				line++;
			}
			in.close();
			Assert::AreEqual(num * 9, line);


			s.out.open("problem_sudoku.txt", ios::out);
			s.generate(num,true,answer_sudoku);
			for (int i = 0; i < num; i++) {
				s.write_file(answer_sudoku[i]);
			}
			s.out.close();
			//TODO:≤‚ ‘Œ®“ª

			s.out.open("problem_sudoku.txt", ios::out);
			s.generate(num, 1, answer_sudoku);
			for (int i = 0; i < num; i++) {
				s.write_file(answer_sudoku[i]);
			}
			Assert::AreEqual(s.getFree() <= 650 && s.getFree() >= 0, true);
			s.out.close();

			s.out.open("problem_sudoku.txt", ios::out);
			s.generate(num, 2, answer_sudoku);
			for (int i = 0; i < num; i++) {
				s.write_file(answer_sudoku[i]);
			}
			Assert::AreEqual(s.getFree() <= 999 && s.getFree() >= 651, true);
			s.out.close();

			s.out.open("problem_sudoku.txt", ios::out);
			s.generate(num, 3, answer_sudoku);
			for (int i = 0; i < num; i++) {
				s.write_file(answer_sudoku[i]);
			}
			Assert::AreEqual(s.getFree() <= 1944 && s.getFree() >= 1000, true);
			s.out.close();

			s.out.open("problem_sudoku.txt", ios::out);
			s.generate(num, lower, higher, answer_sudoku);
			for (int i = 0; i < num; i++) {
				s.write_file(answer_sudoku[i]);
			}
			s.out.close();
			in.open("problem_sudoku.txt", ios::in);
			int emptyCount = 0;
			while (in.getline(buffer, 20)) {
				if (buffer[0] == '\0') {
					Assert::AreEqual(emptyCount <= higher && emptyCount >= lower, true);
					emptyCount = 0;
				}
				for (int i = 0; i < strlen(buffer); i++) {
					if (buffer[i] == '$')emptyCount++;
				}
			}
			in.close();
			
		}

		TEST_METHOD(TestSolve)
		{
			fstream in;
			in.open("generate_sudoku.txt", ios::in);
			s.out.open("sudoku.txt", ios::out);
			Assert::AreEqual(s.solve_problem(in), true);
			in.close();
			s.out.close();

			in.open("generate_sudoku.txt", ios::in);
			char buffer[20];
			fstream in2;
			in2.open("sudoku.txt", ios::in);
			char buffer2[20];
			while (in.getline(buffer, 20)&&in2.getline(buffer2,20)) {
				for (int i = 0; i < strlen(buffer); i++) {
					Assert::AreEqual(buffer[i], buffer2[i]);
				}
			}
			in.close();
			in2.close();

			in.open("problem_sudoku.txt", ios::in);
			s.out.open("sudoku.txt", ios::out);
			Assert::AreEqual(s.solve_problem(in), true);
			in.close();
			s.out.close();
			in.open("problem_sudoku.txt", ios::in);
			in2.open("sudoku.txt", ios::in);
			int empty = 0;
			while (in.getline(buffer, 20) && in2.getline(buffer2, 20)) {
				for (int i = 0; i < strlen(buffer); i++) {
					if (buffer[i] != '$')
						Assert::AreEqual(buffer[i], buffer2[i]);
					if(buffer2[i] == '$') empty++;
				}
			}
			Assert::AreEqual(empty,0);
			in.close();
			in2.close();
		}

		
	};
}
