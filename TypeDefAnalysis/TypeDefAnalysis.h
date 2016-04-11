#pragma once
///////////////////////////////////////////////////////////////////////
// TypeDefAnalysis.h - Defines actions to handle and analysis types. //
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

class TypeDefAnalysis
{
public:
	//using Result_Type = std::map<std::string, std::vector<struct typedefi>>;
	//TypeDefAnalysis(BlockingQueue<Table>& tableQ) : tableQ_(tableQ) {}
	TypeDefAnalysis(std::string path) { _path = path; };
	void doAnalysis();
	std::map<std::string, std::vector<struct typedefi>> gettypetable() { return typetable; }
	std::map<std::string, std::vector<struct typedefi>> getextendtable() { return typetable_extend; }
private:
	std::string _path;
	std::map<std::string, std::vector<struct typedefi>> typetable;
	std::map<std::string, std::vector<struct typedefi>> typetable_extend;
};

