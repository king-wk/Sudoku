#include "pch.h"
#include "CppUnitTest.h"
#include"D:\Software Project\sudoku\para.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SudokuUnitTest
{
	TEST_CLASS(SudokuUnitTest)
	{
	public:
		para check;
		TEST_METHOD(TestParaCount)
		{
			int argc = 0;
			char** argv = NULL;
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "Insufficient argument");

			argv = new char* [6];
			argc = 1;
			argv[1] = "anything";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "Insufficient argument");

			argc = 2;
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "Insufficient argument");

			argc = 6;
			argv[1] = "-n";
			argv[2] = "10";
			argv[3] = "-m";
			argv[4] = "2";
			argv[5] = "-r";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "Wrong number of parameters");
		}

		TEST_METHOD(TestParaCCount)
		{
			int argc = 5;
			char** argv = new char*[5];
			argv[1] = "-c";
			argv[2] = "10";
			argv[3] = "-n";
			argv[4] = "10";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "Excessive arguments of -c");
		}

		TEST_METHOD(TestParaCIsNum)
		{
			int argc = 3;
			char** argv = new char* [3];
			argv[1] = "-c";
			argv[2] = "eee";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "Use a number after -c");
		}

		TEST_METHOD(TestParaCNumRange)
		{
			int argc = 3;
			char** argv = new char* [3];
			argv[1] = "-c";
			argv[2] = "0";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "-c number should be in [1,1000000]");
			Assert::AreEqual(check.getNumber(), 0);

			argv[2] = "1000001";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "-c number should be in [1,1000000]");
			Assert::AreEqual(check.getNumber(), 1000001);

			argv[2] = "50";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), true);
			Assert::AreEqual(check.getRightCode(),(int)GEN);
			Assert::AreEqual(check.getNumber(), 50);
		}

		TEST_METHOD(TestParaSCount)
		{
			int argc = 4;
			char** argv = new char* [4];
			argv[1] = "-s";
			argv[2] = "0";
			argv[3] = "-n";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "Excessive arguments of -s");
		}

		TEST_METHOD(TestParaSPath)
		{
			int argc = 3;
			char** argv = new char* [3];
			argv[1] = "-s";
			argv[2] = "D:\\sudoku.txt";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "-s file doesn't exist");

			argv[2] = "..\\Debug\\sudoku.txt";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), true);
			Assert::AreEqual(check.getRightCode(), (int)SOLVE);
		}

		TEST_METHOD(TestParaNIsNum)
		{
			int argc = 3;
			char** argv = new char* [3];
			argv[1] = "-n";
			argv[2] = "eee";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "Use a number after -n");
		}

		TEST_METHOD(TestParaNNumRange)
		{
			int argc = 3;
			char** argv = new char* [3];
			argv[1] = "-n";
			argv[2] = "0";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "-n number should be in [1,10000]");
			Assert::AreEqual(check.getNumber(), 0);

			argv[2] = "10001";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "-n number should be in [1,10000]");
			Assert::AreEqual(check.getNumber(), 10001);

			argv[2] = "50";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), true);
			Assert::AreEqual(check.getRightCode(), (int)GAME);
			Assert::AreEqual(check.getNumber(), 50);
		}

		TEST_METHOD(TestParaU)
		{
			int argc = 4;
			char** argv = new char* [4];
			argv[1] = "-n";
			argv[2] = "50";
			argv[3] = "-h";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "Parameter error");

			argv[3] = "-u";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), true);
			Assert::AreEqual(check.getRightCode(), (int)UNI);
		}

		TEST_METHOD(TestParaM)
		{
			int argc = 5;
			char** argv = new char* [5];
			argv[1] = "-n";
			argv[2] = "50";
			argv[3] = "-m";
			argv[4] = "85";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "-m number should be 1 or 2 or 3");

			argv[4] = "4";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "-m number should be 1 or 2 or 3");

			argv[4] = "2";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), true);
			Assert::AreEqual(check.getRightCode(), (int)RANK);
			Assert::AreEqual(check.getMode(), 2);
		}

		TEST_METHOD(TestParaR)
		{
			int argc = 5;
			char** argv = new char* [5];
			argv[1] = "-n";
			argv[2] = "50";
			argv[3] = "-r";
			argv[4] = "r1~r2";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "-r r1~r2 must be int");

			argv[4] = "2055";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "-r r1~r2, there must be a ~");

			argv[4] = "0~64";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(),"-r r1~r2 should be in [1, 64] and r1<=r2");

			argv[4] = "1~65";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "-r r1~r2 should be in [1, 64] and r1<=r2");

			argv[4] = "55~20";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "-r r1~r2 should be in [1, 64] and r1<=r2");


			argv[4] = "20~55";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), true);
			Assert::AreEqual(check.getRightCode(), (int)DIG);
			Assert::AreEqual(check.getLower(), 20);
			Assert::AreEqual(check.getUpper(), 55);

			argv[3] = "-k";
			argv[4] = "anything";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "Parameter error");
		}

		TEST_METHOD(TestParaOther)
		{
			int argc = 3;
			char** argv = new char* [3];
			argv[1] = "-m";
			argv[2] = "2";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "must be with -n before -m or -r");

			argv[1] = "-r";
			argv[2] = "20~55";
			check.setPara(argc, argv);
			Assert::AreEqual(check.check(), false);
			Assert::AreEqual(check.getWrongCode().c_str(), "must be with -n before -m or -r");
		}
	};
}
