///////////////////////////////////////////////////////////////////////
// ParaDepExe.cpp - Program entry point for the parallel dependency  // 
//                  analysis program.                                //
//															         //
//																	 //	
// Tianhang Zhang, CSE687 - Object Oriented Design, Spring 2016      //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
*This pakecage provides the entry point for the whole program
*/

#include <thread>
#include <functional>
#include <set>
#include <algorithm>
#include <windows.h>
#include "ParaDepExe.h"

#ifdef MAIN_ENTRY
void main()
{
	//std::cout << "-----------------------------------------" << "\n";
	//std::cout << "-----------------Req# 6------------------" << "\n";
	//std::cout << "Running type definition analysis on test case testcase.cpp:  \n\n";

	//std::string inputdir1 = "..";
	//std::vector<std::string> pat1 = { "testcase.cpp" };

	//ParaDepExe* p1 = new ParaDepExe(inputdir1);
	//p1->retrievePaths(pat1);
	//p1->startTypeAnalysis();
	//Sleep(500);
	//p1->~ParaDepExe();


	//std::cout << "-----------------------------------------" << "\n";
	//std::cout << "-----------------Req# 7------------------" << "\n";
	//std::cout << "Running type definition analysis and dependency analysis in current dir:  \n\n";
	//std::string inputdir2 = ".";
	//std::vector<std::string> pat2 = { "*.cpp", "*.h" };

	////p1->setDir(inputdir2);
	////p1->retrievePaths(pat2);
	////p1->startTypeAnalysis();
	//ParaDepExe* p2 = new ParaDepExe(inputdir2);
	//p2->retrievePaths(pat2);
	//p2->startTypeAnalysis();
	//p2->startDepAnalysis();
	//p2->~ParaDepExe();

	std::cout << "----------------------------------------------" << "\n";
	std::cout << "---------------Req# 6, 7, 8, 9----------------" << "\n";
	std::cout << "Running type definition analysis and dependency analysis in upper dir for .cpp and .h files:  \n\n";
	std::string inputdir3 = "..";
	std::vector<std::string> pat3 = { "*.cpp", "*.h" };

	ParaDepExe* p3 = new ParaDepExe(inputdir3);
	p3->retrievePaths(pat3);
	p3->startTypeAnalysis();
	p3->startDepAnalysis();
	p3->~ParaDepExe();
}
#endif