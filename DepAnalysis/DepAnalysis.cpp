///////////////////////////////////////////////////////////////////////
// DepAnalysis.h - Defines actions to handle and analysis            //
//                 dependencies.                                     //
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
#include "DepAnalysis.h"
#include "../Parser/ConfigureParser.h"
#include "../Parser/Parser.h"

using File = std::string;
using Table = std::string;

void DepAnalysis::doAnalysis()
{
	ConfigParseToConsole configure;
	Parser* pParser = configure.BuildDepAnal(_path);
	try
	{
		if (pParser)
		{
			if (!configure.Attach(_path))
			{
				std::cout << "\n could not open file " << _path << std::endl;
				return;
			}
		}
		else
		{
			std::cout << "\n\n Parser not built\n\n";
			return;
		}
		configure.acceptExTypeTable(typetable_extend);
		while (pParser->next())
			pParser->parse();
		//std::cout << "\n\n";
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	DepPair = configure.getDependInfo();
	//std::cout << "\n\n";
}

#ifdef TEST_DEPANALYSIS
int main()
{
	std::cout << "--------------------------------------------------";
	std::cout << "Testing type definition analysis in single thread.   \n\n";
	std::cout << "\n\n";

	std::string path = "TypeDefAnalysis.h";
	TypeDefAnalysis typeDefAnal(path);
	typeDefAnal.doAnalysis();
	//std::map<std::string, std::vector<struct typedefi>> result = typeDefAnal.gettypetable();
}
#endif