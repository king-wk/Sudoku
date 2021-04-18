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
			number = 2;
			s.out.open("C://Users//PiKing//source//repos//Sudoku//Debug//sudoku.txt", ios::out);
			s.generate_final(number, 1);
			fstream in;
			in.open("C://Users//PiKing//source//repos//Sudoku//Debug//sudoku.txt", ios::in);
			char buffer[20];
			int line = 0;
			Assert::AreEqual(in.getline(buffer, 20).good(), true);
			while (!in.eof()) {
				//getline(in, buffer);
				line++;
			}
			in.close();
			string s = "a";
			//Assert::AreEqual(r, s);
			Assert::AreEqual(number*10-1, line);
		}

		TEST_METHOD(TestGenRule)
		{
			srand((unsigned int)time(0));
			int number = rand() % 10 + 1;
			s.out.open("generate_sudoku.txt", ios::out);
			// 生成数独终局
			s.generate_final(number, 1);
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
			s.out.open("problem_sudoku.txt", ios::out);
			s.generate(num, answer_sudoku);
			for (int i = 0; i < num; i++) {
				s.write_file(answer_sudoku[i]);
			}
			//TODO:测试数量

			s.generate(num,true,answer_sudoku);
			for (int i = 0; i < num; i++) {
				s.write_file(answer_sudoku[i]);
			}
			//TODO:测试唯一

			s.generate(num, 1, answer_sudoku);
			for (int i = 0; i < num; i++) {
				s.write_file(answer_sudoku[i]);
			}

			s.generate(num, 2, answer_sudoku);
			for (int i = 0; i < num; i++) {
				s.write_file(answer_sudoku[i]);
			}

			s.generate(num, 3, answer_sudoku);
			for (int i = 0; i < num; i++) {
				s.write_file(answer_sudoku[i]);
			}
			//TODO 测试难度

			s.generate(num, lower, higher, answer_sudoku);
			for (int i = 0; i < num; i++) {
				s.write_file(answer_sudoku[i]);
			}
			//TODO 测试挖空数
		}

		TEST_METHOD(TestSolve)
		{
			fstream in;
			in.open("generate_sudoku.txt", ios::in);
			s.out.open("sudoku.txt", ios::out);
			Assert::AreEqual(s.solve_problem(in), true);
			in.close();

			in.open("problem_sudoku.txt", ios::in);
			s.out.open("sudoku.txt", ios::out);
			Assert::AreEqual(s.solve_problem(in), true);
			in.close();
		}

		
	};
}
