///////////////////////////////////////////////////////////////////////
// ParaDepAnalysis.cpp - Evaluates the dependency graph for all the  //
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
#include <set>
#include <algorithm>
#include "ParaDepAnalysis.h"
#include "../Parser/ConfigureParser.h"
#include "../Parser/Parser.h"


void ParaDepAnalysis::doParaTypeAnalysis()
{
	task_TypeAnal->startwork();
	static std::mutex m;
	static int count_type = 0; 
	count_type = 0;
	static std::vector<std::string> _paths = paths;
	static std::function<std::map<std::string, std::vector< typedefi>>()> obj = []()
	{
		m.lock();
		std::string cur_path = _paths[count_type];
		std::cout << "\n working on thread " << std::this_thread::get_id() << "   number:" << count_type << "\n";
		std::cout << "    path: " << cur_path;
		m.unlock();

		TypeDefAnalysis typeDefAnal(cur_path);
		typeDefAnal.doAnalysis();
		std::map<std::string, std::vector< typedefi>> TypeTable = typeDefAnal.gettypetable();
		std::map<std::string, std::vector< typedefi>> TypeTableExtend = typeDefAnal.getextendtable();
		count_type++;
		return TypeTableExtend;
	};

	for (auto _path : paths)
		task_TypeAnal->addWork(&obj);
	std::map<std::string, std::vector< typedefi>> Re_TypeTableExtend;
	std::vector<std::map<std::string, std::vector< typedefi>>> allTypeTableExtend;
	while(true)
	{
		int numre = task_TypeAnal->NumResult();
		int numwork = task_TypeAnal->NumWork();
		if (task_TypeAnal->NumResult() == task_TypeAnal->NumWork())
		{
			for (int i = 0; i < 4; i++)
				task_TypeAnal->addWork(nullptr);
			for (int i = 0; i < numre; i++)
			{
				Re_TypeTableExtend = task_TypeAnal->getResult();
				allTypeTableExtend.push_back(Re_TypeTableExtend);
			}
			mergeTypeTable(allTypeTableExtend);
			break;
		}
	}
	task_TypeAnal->join();
	showTypeTable();
	//showExtendedTypeTable();
}

void ParaDepAnalysis::mergeTypeTable(std::vector<std::map<std::string, std::vector< typedefi>>> alltypetable)
{
	std::map<std::string, std::vector< typedefi>> _finalTypeTableExtend;
	for (auto table : alltypetable)
	{
		for (auto pair : table)
		{
			if (_finalTypeTableExtend.find(pair.first) == _finalTypeTableExtend.end())
				_finalTypeTableExtend.insert(pair);
			else
			{
				std::vector< typedefi>* tmpStruct = &_finalTypeTableExtend[pair.first];
				std::vector< typedefi> tmpStruct1 = pair.second;
				_finalTypeTableExtend[pair.first].insert(tmpStruct->begin(), tmpStruct1.begin(), tmpStruct1.end());
			}
		}
	}
	finalTypeTableExtend = _finalTypeTableExtend;
	for (auto pair : _finalTypeTableExtend)
	{
		std::vector<struct typedefi> t = pair.second;
		for (int i = 0; i < t.size(); i++)
		{
			if (t[i].type == "function" || t[i].type == "namespace")
				t.erase(t.begin() + i);
		}
		if (t.size() > 0)
			finalTypeTable.insert(pair);
	}
}

void ParaDepAnalysis::doParaDepAnalysis()
{
	task_DepAnal->startwork();
	static int count_dep = 0;
	static std::vector<std::string> _paths = paths;
	count_dep = 0;
	static std::map<std::string, std::vector< typedefi>> _finalTypeTableExtend = finalTypeTableExtend;
	static std::function<std::pair<std::string, depAnali>()> obj = []()
	{
		std::string cur_path = _paths[count_dep++];
		std::cout << "\n working on thread " << std::this_thread::get_id() << "   number:" << count_dep << "\n";
		std::cout << "    path: " << cur_path;

		DepAnalysis DepAnal(cur_path, _finalTypeTableExtend);
		DepAnal.doAnalysis();
		//std::map<std::string, std::vector<struct typedefi>> TypeTable = typeDefAnal.gettypetable();
		std::pair<std::string,  depAnali> DepPair = DepAnal.getDepInfo();

		return DepPair;
	};

	for (auto _path : paths)
		task_DepAnal->addWork(&obj);

	std::pair<std::string,  depAnali> Re_DepPair;
	std::vector<std::pair<std::string,  depAnali>> DepTable;
	while (true)
	{
		int numre = task_DepAnal->NumResult();
		int numwork = task_DepAnal->NumWork();
		if (task_DepAnal->NumResult() == task_DepAnal->NumWork())
		{
			for (int i = 0; i < 4; i++)
				task_DepAnal->addWork(nullptr);
			for (int i = 0; i < numre; i++)
			{
				Re_DepPair = task_DepAnal->getResult();
				DepTable.push_back(Re_DepPair);
			}
			mergeDepTable(DepTable);
			break;
		}
	}
	task_DepAnal->join();
	showDependTable();
}

void ParaDepAnalysis::mergeDepTable(std::vector<std::pair<std::string,  depAnali>> allDepPair)
{
	std::map<std::string,  depAnali> _finalDepTable;
	for (auto pair : allDepPair)
	{
		if (_finalDepTable.find(pair.first) == _finalDepTable.end())
			_finalDepTable.insert(pair);
	}
	for (auto pair : _finalDepTable)
	{
		//std::vector<std::string>* t = &pair.second.depFiles;
		std::set<std::string> s(pair.second.depFiles.begin(), pair.second.depFiles.end());
		pair.second.depFiles.clear();
		pair.second.depFiles.assign(s.begin(), s.end());

		//for (int i = 0; i < pair.second.depFiles.size(); i++)
		//{
		//	if (pair.second.depFiles[i] == "")
		//		pair.second.depFiles.erase[i];
		//}
		finalDepTable.insert(pair);
	}
	//finalDepTable = _finalDepTable;
}

void ParaDepAnalysis::showTypeTable()
{
	std::cout << "\n\n --------------------------------------------\n";
	std::cout << "Display TypeTable: " << "\n";
	for (auto TPair : finalTypeTable)
	{
		std::vector< typedefi> TPinfo = TPair.second;
		for each ( typedefi t in TPinfo)
		{
			if (t.type != "function" && t.type != "namespace")
			{
				std::cout << "            type name: " << TPair.first << "\n";
				std::cout << "                 type: " << t.type << "\n";
				std::cout << "           defined in: " << t.inFile << "\n";
				std::cout << "           total size: " << t.lineCount << "\n";
				std::cout << "belongs to namespaces: ";
				for (auto ns : t.inNamespaces)
					//std::cout << "                       " << ns << "\n";
					std::cout << " " << ns << ",";
				std::cout << "\n\n";
			}
		}

	}
}

void ParaDepAnalysis::showExtendedTypeTable()
{
	std::cout << "\n\n --------------------------------------------\n";
	std::cout << "Display Extended TypeTable: " << "\n";
	for (auto TPair : finalTypeTableExtend)
	{
		std::vector< typedefi> TPinfo = TPair.second;
		for each ( typedefi t in TPinfo)
		{
			std::cout << "            type name: " << TPair.first << "\n";
			std::cout << "                 type: " << t.type << "\n";
			std::cout << "           defined in: " << t.inFile << "\n";
			std::cout << "           total size: " << t.lineCount << "\n";
			std::cout << "belongs to namespaces: ";
			for (auto ns : t.inNamespaces)
				//std::cout << "                       " << ns << "\n";
				std::cout << " " << ns << ",";
			std::cout << "\n\n";
		}

	}
}

void ParaDepAnalysis::showDependTable()
{
	std::cout << "\n\n --------------------------------------------\n";
	std::cout << "Display Dependency Table: " << "\n";
	for (auto pair : finalDepTable)
	{
		struct depAnali d = pair.second;
		std::cout << "      file name: " << pair.first << "\n";
		std::cout << "depend on files:   ";
		if (d.depFiles.size() > 0)
			std::cout << d.depFiles[0] << ",  ";
		if (d.depFiles.size() > 1)
		{
			for (int i = 1; i < d.depFiles.size(); i++)
				std::cout << "\n" << "                   " << d.depFiles[i] << ",  ";
		}
		std::cout << "\n\n";
	}
}

void ParaDepAnalysis::retrievePaths(std::vector<std::string> patterns)
{
	FileMgr fm(path);
	for (auto p : patterns)
	{
		fm.addPattern(p);
	}
	fm.search();
	paths = fm.getStores();
	std::cout << "-------------------------------------------------\n";
	std::cout << "File Manager:    \n";
	std::cout << "Parsing Files:  \n";
	for (auto s : paths)
		std::cout << "                " << s << "\n";
}

#ifdef TEST_PARADEPANALYSIS
int main()
{
	std::cout << "Testing Package ParaDepAnalysis.   " << "\n";
	std::string inputpath;
	inputpath = "..";

	ParaDepAnalysis* ParaDAnal = new ParaDepAnalysis(inputpath);
	//ParaDAnal->addPattern(".cpp");
	//ParaDAnal->addPattern(".h");
	ParaDAnal->retrievePaths({"*.cpp", "*.h"});
	ParaDAnal->doParaTypeAnalysis();
	ParaDAnal->doParaDepAnalysis();
}
#endif