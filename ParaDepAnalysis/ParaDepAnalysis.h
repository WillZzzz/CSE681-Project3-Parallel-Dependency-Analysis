#pragma once
///////////////////////////////////////////////////////////////////////
// ParaDepAnalysis.h - Evaluates the dependency graph for all the    //
//                      packages in a specified file collection.     //
//															         //
//																	 //	
// Tianhang Zhang, CSE687 - Object Oriented Design, Spring 2016      //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
*For each file the analyses run asynchronously, using the facilities of 
*the Task class. I expect you will find it easiest to implement this in 
*two passes over the file collection. In the first pass we find, 
*asynchronously, all the types defined in the collection using services 
*of the TypeAnalysis package. In the second pass we find, asynchronously, 
*all the dependencies between files using the DependencyAnalysis package.
*/

#include <thread>
#include <functional>
#include "../TypeDefAnalysis/TypeDefAnalysis.h"
#include "../DepAnalysis/DepAnalysis.h"
#include "../Parser/ConfigureParser.h"
#include "../Parser/Parser.h"
#include "../FileMgr/FileMgr.h"
#include "../DataStore/DataStore.h"
#include "../Task/Task.h"

class ParaDepAnalysis
{
	//using Result_Type = std::map<std::string, std::vector<struct typedefi>>;
	//using Result_Dep = std::pair<std::string, std::vector<struct depAnali>>;
public:
	ParaDepAnalysis(std::string _path) { 
		path = _path; 
		task_TypeAnal = new Task<std::map<std::string, std::vector<struct typedefi>>>();
		task_DepAnal = new Task<std::pair<std::string, struct depAnali>>();
		//fm = new FileMgr(path);
	}
	void setDir(std::string dir) { path = dir; }
	void doParaTypeAnalysis();
	void doParaDepAnalysis();
	void mergeTypeTable(std::vector<std::map<std::string, std::vector<struct typedefi>>> alltypetable);
	void mergeDepTable(std::vector<std::pair<std::string, struct depAnali>> allDepPair);
	std::map<std::string, std::vector<struct typedefi>> getTypeTable() { return finalTypeTable; }
	std::map<std::string, struct depAnali> getDepTable() { return finalDepTable; }
	void showTypeTable();
	void showExtendedTypeTable();
	void showDependTable();
	//void addPattern(std::string pattern) { fm->addPattern(pattern); }
	void retrievePaths(std::vector<std::string> patterns);
	~ParaDepAnalysis() {
		//task_DepAnal->join();
		//task_TypeAnal->join();
		delete task_DepAnal;
		delete task_TypeAnal;
	}
private:
	std::string path;
	std::vector<std::string> paths;
	//TypeDefAnalysis* pTypeAnal;
	//DepAnalysis* pDepAnal;
	//FileMgr* fm;
	Task<std::map<std::string, std::vector<struct typedefi>>>* task_TypeAnal;
	Task<std::pair<std::string, struct depAnali>>* task_DepAnal;
	std::map<std::string, std::vector<struct typedefi>> finalTypeTable;
	std::map<std::string, std::vector<struct typedefi>> finalTypeTableExtend;
	std::map<std::string, struct depAnali> finalDepTable;
};

