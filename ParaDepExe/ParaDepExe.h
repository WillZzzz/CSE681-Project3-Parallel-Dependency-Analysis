#pragma once
///////////////////////////////////////////////////////////////////////
// ParaDepExe.h - Program entry point for the parallel dependency    // 
//                  analysis program.                                //
//															         //
//																	 //	
// Tianhang Zhang, CSE687 - Object Oriented Design, Spring 2016      //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This pakecage provides the entry point for the whole program.
* Searching Dir is given in the construction method.
* Can add patterns in the retrievePaths() method in the form of 
* vector of strings.
*/

#include <thread>
#include <functional>
#include <set>
#include <algorithm>
#include "../ParaDepAnalysis/ParaDepAnalysis.h"

class ParaDepExe
{
private:
	ParaDepAnalysis* ParaD;
public:
	ParaDepExe(std::string startingdir) { ParaD = new ParaDepAnalysis(startingdir);}
	void retrievePaths(std::vector<std::string> patterns) { ParaD->retrievePaths(patterns); }
	void startTypeAnalysis() { ParaD->doParaTypeAnalysis(); }
	void startDepAnalysis() { ParaD->doParaDepAnalysis(); }
	void setDir(std::string dir) { ParaD->setDir(dir); }
	~ParaDepExe() { delete ParaD; }
};
