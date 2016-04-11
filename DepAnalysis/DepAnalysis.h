#pragma once
///////////////////////////////////////////////////////////////////////
// DepAnalysis.h - Defines actions to handle and analysis            //
//                 dependencies.                                     //
//															         //
//																	 //	
// Tianhang Zhang, CSE687 - Object Oriented Design, Spring 2016      //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
*Defines actions to parse, handle and analysis all the type definitions
*in files. Adopts services provided by ThreadPool to execute those
*analysis asynchronously.
*
*Package generates a type table, a merged table of results from each
*thread in ThreadPool.
*/

#include <thread>
#include <functional>
#include <map>
#include <string>
#include "../FileMgr/FileMgr.h"
#include "../ThreadPool/BlockingQueue.h"
#include "../Task/Task.h"
#include "../Parser/ActionsAndRules.h"

class DepAnalysis
{
public:
	using File = std::string;
	using Table = std::string;
	//TypeDefAnalysis(BlockingQueue<Table>& tableQ) : tableQ_(tableQ) {}
	DepAnalysis(std::string path, std::map<std::string, std::vector<typedefi>> extendtable) { _path = path; typetable_extend = extendtable; }
	void doAnalysis();
	std::pair<std::string, depAnali> getDepInfo() { return DepPair; }
private:
	std::string _path;
	std::map<std::string, std::vector<typedefi>> typetable_extend;
	std::pair<std::string, depAnali> DepPair;
};

