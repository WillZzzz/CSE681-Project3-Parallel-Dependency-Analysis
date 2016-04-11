///////////////////////////////////////////////////////////////////////
// TypeDefAnalysis.cpp - Defines actions to handle and analysis      //
//							types.								     //
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
#include "TypeDefAnalysis.h"
#include "../Parser/ConfigureParser.h"
#include "../Parser/Parser.h"

void TypeDefAnalysis::doAnalysis()
{
	ConfigParseToConsole configure;
	Parser* pParser = configure.BuildTypeDef(_path);
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
		while (pParser->next())
			pParser->parse();
		//std::cout << "\n\n";
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	typetable = configure.getTypeTable();
	typetable_extend = configure.getExtendTable();

}

#ifdef TEST_TYPEDEFANALYSIS
int main()
{
	std::cout << "--------------------------------------------------";
	std::cout << "Testing type definition analysis in single thread.   \n\n";
	std::cout << "\n\n";

	std::string path = "TypeDefAnalysis.h";
	TypeDefAnalysis typeDefAnal(path);
	typeDefAnal.doAnalysis();
	std::map<std::string, std::vector<struct typedefi>> result = typeDefAnal.gettypetable();
}
#endif