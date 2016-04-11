#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions     //
//  ver 2.2                                                        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
// Author:        Tianhang Zhang, Syracuse University			   //
//                (315) 383-3757, tzhan116@syr.edu		           //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module defines several action classes.  Its classes provide 
  specialized services needed for specific applications.  The modules
  Parser, SemiExpression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable. 

  Public Interface:
  =================
  Toker t(someFile);              // create tokenizer instance
  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
  Parser parser(se);              // now we have a parser
  Rule1 r1;                       // create instance of a derived Rule class
  Action1 a1;                     // create a derived action
  r1.addAction(&a1);              // register action with the rule
  parser.addRule(&r1);            // register rule with parser
  while(se.getSemiExp())          // get semi-expression
    parser.parse();               //   and parse it

  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
      ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
    - devenv CodeAnalysis.sln
    - cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
         semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================
  ver 2.3 : 09 April 16
  - Addtional rules and actions to handle type definitions and using of namespaces.
  ver 2.2 : 14 March 16
  - Additional rules and actions to detect other scopes and handle abstract syntax tree
  ver 2.1 : 15 Feb 16
  - small functional change to a few of the actions changes display strategy
  - preface the (new) Toker and SemiExp with Scanner namespace
  ver 2.0 : 01 Jun 11
  - added processing on way to building strong code analyzer
  ver 1.1 : 17 Jan 09
  - changed to accept a pointer to interfaced ITokCollection instead
    of a SemiExpression
  ver 1.0 : 12 Jan 06
  - first release

*/
//
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "../SemiExp/itokcollection.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "AbstractSyntaxTree.h"
#include "../ScopeStack/ScopeStack.h"

///////////////////////////////////////////////////////////////
// ScopeStack element is application specific
/* ToDo:
 * - chanage lineCount to two fields: lineCountStart and lineCountEnd
 */

struct element
{
	std::string type;
	std::string name;
	int lineCount;
	std::string show()
	{
		std::ostringstream temp;
		temp << "(";
		temp << type;
		temp << ", ";
		temp << name;
		temp << ", ";
		temp << lineCount;
		temp << ")";
		return temp.str();
	}
};

struct typedefi
{
  std::string type;
  int lineCount;
  std::string inFile;
  std::vector<std::string> inNamespaces;
};

struct depAnali
{
	std::vector<std::string> depFiles;
};

///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.
/*
 * ToDo:
 * - add AST Node class
 * - provide field to hold root of AST
 * - provide interface to access AST
 * - provide helper functions to build AST, perhaps in Tree class
 */
class Repository  // application specific
{
  std::map<std::string, std::vector<struct typedefi>> typetable;
  // extend typetable contains namespace and function definition,
  // which will be looked for in further analysis, dependency 
  // analysis.
  std::map<std::string, std::vector<struct typedefi>> typetable_extend;
  //std::map<std::string, struct depAnali> dependtable;
  std::pair<std::string, struct depAnali> dependPair;
  struct depAnali dependinfo;
  ScopeStack<Node *> stack;
  Scanner::Toker* p_Toker;
  std::string path;
public:
  Repository(Scanner::Toker* pToker)
  {
    p_Toker = pToker;
  }
  Repository(Scanner::Toker* pToker, std::string _path)
  {
	  p_Toker = pToker;
	  path = _path;
  }
  std::string getFileName()
  {
	  return path;
  }
  std::map<std::string, std::vector<struct typedefi>>& gettypetable()
  {
    return typetable;
  }
  std::map<std::string, std::vector<struct typedefi>>& getextendtable()
  {
	  return typetable_extend;
  }
  void setextendtable(std::map<std::string, std::vector<struct typedefi>> extendtable)
  {
	  typetable_extend = extendtable;
  }
  void addDepend(std::string path)
  {
	  dependinfo.depFiles.push_back(path);
  }
  std::pair<std::string, struct depAnali> getDependinfo()
  {
	  dependPair = std::make_pair(path, dependinfo);
	  return dependPair;
  }
  ScopeStack<Node *>& scopeStack()
  {
	  return stack;
  }
  Scanner::Toker* Toker()
  {
    return p_Toker;
  }
  int lineCount()
  {
    return (int)(p_Toker->currentLineCount());
  }
  void addTypeDef(std::string type_name, std::vector<struct typedefi> typeinfos)
  {
	  std::pair<std::map<std::string, std::vector<struct typedefi>>::iterator, bool> ret;
	  ret = typetable.insert(std::pair<std::string, std::vector<struct typedefi>>(type_name, typeinfos));
	  if (ret.second == false)		// indicates that the key already exists
	  {
		  for each (struct typedefi typeinfo in typeinfos)
		  {
			  ret.first->second.push_back(typeinfo);
		  }
	  }
  }
  void addExtendTypeDef(std::string type_name, std::vector<struct typedefi> typeinfos)
  {
	  std::pair<std::map<std::string, std::vector<struct typedefi>>::iterator, bool> ret;
	  ret = typetable_extend.insert(std::pair<std::string, std::vector<struct typedefi>>(type_name, typeinfos));
	  if (ret.second == false)		// indicates that the key already exists
	  {
		  for each (struct typedefi typeinfo in typeinfos)
		  {
			  ret.first->second.push_back(typeinfo);
		  }
	  }
  }
};

///////////////////////////////////////////////////////////////
// rule to detect beginning of type definitions, including class,
// structure, enumerator and aliase

class BeginningOfTypeDef : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
	const static std::string keys[]
		= { "class", "struct", "enum", "using" };
	for (int i = 0; i < 4; i++)
	{
		if (pTc->find(keys[i]) < pTc->length() && pTc->find("namespace") >= pTc->length() && pTc->find("{") < pTc->length())
		{
			if (!(pTc->find("struct") < pTc->length() && pTc->find("(") < pTc->length()))
			{
				doActions(pTc);
				return true;
			}
		}
		if (pTc->find("using") < pTc->length() && pTc->find("namespace") >= pTc->length())
		{
			doActions(pTc);
			return true;
		}
	}
	return true;
	}
};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("{") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
	Repository* p_Repos;
	AbstractSyntaxTree* p_AST;
public:
	HandlePush(Repository* pRepos, AbstractSyntaxTree* pAST)
	{
		p_Repos = pRepos;
		p_AST = pAST;
	}
	void doAction(ITokCollection*& pTc)
	{
		//std::cout << "\n--BeginningOfScope rule";
		Node* node = new Node("unknown", "anonymous", p_Repos->lineCount(), std::vector<Node*> {});
		node->setStartLine(p_Repos->lineCount());
		Node* root = p_AST->getRoot();
		if (p_Repos->scopeStack().size() == 0)
		{
			p_Repos->scopeStack().push(root);
		}
		p_Repos->scopeStack().push(node);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("}") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
	Repository* p_Repos;
public:
	HandlePop(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//std::cout << "\n--EndOfScope rule";
		if (p_Repos->scopeStack().size() == 0)
			return;
		//element elem = p_Repos->scopeStack().pop();
		Node* node = p_Repos->scopeStack().pop();
		node->setEndLine(p_Repos->lineCount());
		//if (node->getType() == "function")
		//{
		//	std::cout << "\n  Function " << node->getName() << ", lines = " << p_Repos->lineCount() - node->getLineCount() + 1;
		//	std::cout << "\n";
		//}
	}
};

///////////////////////////////////////////////////////////////
// action to create a new pair and add it to the map

class AddPair : public IAction
{
	Repository* p_Repos;
	ScopeStack<Node *> p_Stack;
public:
	AddPair(Repository* pRepos)
	{
		p_Repos = pRepos;
		p_Stack = pRepos->scopeStack();
	}
	void doAction(ITokCollection*& pTc)
	{
		const static std::string keys[]
			= { "class", "struct", "enum", "using" };
		struct typedefi typeD;
		std::string name;
		ScopeStack<Node*> cur_stack = p_Repos->scopeStack();

		for (int i = 0; i < 3; i++)
		{
			if (pTc->find(keys[i]) < pTc->length())
				typeD.type = keys[i];
		}
		if (pTc->find(keys[3]) < pTc->length())
		{
			typeD.type = "aliase";
			int lenA = pTc->find("using");
			for (int i = lenA; i < pTc->length(); i++)
				name = name + (*pTc)[i];
		}
		for each (Node* node in cur_stack)
		{
			if (node->getType() == "namespace")
				typeD.inNamespaces.push_back(node->getName());
		}
		typeD.lineCount = p_Repos->lineCount();
		if (typeD.type != "aliase")
		{
			int len = pTc->find("{");
			if (pTc->find("{") - 1 == pTc->find(typeD.type))
				name = "anonymous";
			else
				name = (*pTc)[pTc->find("{") - 2];
		}
		typeD.inFile = p_Repos->getFileName();
		std::vector<struct typedefi> typeinfo;
		typeinfo.push_back(typeD);
		p_Repos->addTypeDef(name, typeinfo);
		p_Repos->addExtendTypeDef(name, typeinfo);
	}
};

///////////////////////////////////////////////////////////////
// Rule to detect namespace definitions

class NameSpaceDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			int len = tc.find("namespace");
			if (len < tc.length())
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// Actions to push namespace on to the scopestack

class PushNSDefinition : public IAction
{
	Repository* p_Repos;
public:
	PushNSDefinition(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// next statement is now  handled in PrintFunction
		// std::cout << "\n  FunctionDef: " << pTc->show();

		// pop anonymous scope
		p_Repos->scopeStack().pop();

		// push function scope
		std::string type = "namespace";
		std::string name;
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1].length() == 0)
			name = "anonymous";
		else
			name = tc[tc.length() - 3];
		Node* node = new Node(type, name, p_Repos->lineCount(), std::vector<Node*> {});
		p_Repos->scopeStack().push(node);
	}
};

///////////////////////////////////////////////////////////////
// action to add namespace to the extend typetable for further use

class addNameSpace : public IAction
{
private: 
	Repository* p_Repos;
public:
	addNameSpace(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		struct typedefi typeD;
		std::string name;
		typeD.type = "namespace";
		typeD.inFile = p_Repos->getFileName();
		ITokCollection& tc = *pTc;
		name = tc[tc.length() - 3];
		ScopeStack<Node*> cur_stack = p_Repos->scopeStack();
		int count = 1;
		for each (Node* node in cur_stack)
		{
			if (node->getType() == "namespace" && count < cur_stack.size())
				typeD.inNamespaces.push_back(node->getName());
			count++;
		}
		typeD.lineCount = p_Repos->lineCount();
		std::vector<struct typedefi> typeinfo;
		typeinfo.push_back(typeD);
		p_Repos->addExtendTypeDef(name, typeinfo);
	}
};


///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" , "each", "else", "else if" };
		for (int i = 0; i<8; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}

	bool isLambda(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			int len_round = tc.find("(");
			int len_angle = tc.find("[");
			if (len_angle < len_round && len_round < tc.length() - 1)		// such a structure with "{" and [] in front of () would be a lambda
				return true;
			if (tc.find("<") < tc.length())
				return true;
		}
		return false;
	}

	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			int len = tc.find("(");
			if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]) && !isLambda(pTc))
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
	Repository* p_Repos;
public:
	PushFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// next statement is now  handled in PrintFunction
		// std::cout << "\n  FunctionDef: " << pTc->show();

		// pop anonymous scope
		p_Repos->scopeStack().pop();

		// push function scope

		Node* node = new Node("function", (*pTc)[pTc->find("(") - 1], p_Repos->lineCount(), std::vector<Node*> {});
		p_Repos->scopeStack().push(node);
	}
};

///////////////////////////////////////////////////////////////
// action to add function definition to the extend typetable 
// for further use

class addFunction : public IAction
{
private:
	Repository* p_Repos;
public:
	addFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		struct typedefi typeD;
		std::string name;
		typeD.type = "function";
		typeD.inFile = p_Repos->getFileName();
		ITokCollection& tc = *pTc;
		name = (*pTc)[pTc->find("(") - 1];
		ScopeStack<Node*> cur_stack = p_Repos->scopeStack();
		for each (Node* node in cur_stack)
		{
			if (node->getType() == "namespace")
				typeD.inNamespaces.push_back(node->getName());
		}
		typeD.lineCount = p_Repos->lineCount();
		std::vector<struct typedefi> typeinfo;
		typeinfo.push_back(typeD);
		p_Repos->addExtendTypeDef(name, typeinfo);
	}
};

///////////////////////////////////////////////////////////////////////////
// Actions and rules for handling dependency analysis.
// Basic idea is to look for declarations and using namespaces.
// - In declarations, keep the type and name, then look up the type in 
//   extend typetable for a match.
// - In using namespaces, look up the namespace in extend typetable for
//   a match.

///////////////////////////////////////////////////////////////
// rule to detect declaration

class Declaration : public IRule          // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
	bool isModifier(const std::string& tok) // modifiers and initializers.
	{                                       // If you have two things left
		const int numKeys = 22;            // its declar else executable.
		const static std::string keys[numKeys] = {
			"const", "extern", "friend", "mutable", "signed", "static",
			"typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
			"public", "protected", "private", ":", "typename", "typedef", "++", "--"
		};
		for (int i = 0; i<numKeys; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void condenseTemplateTypes(ITokCollection& tc)
	{
		int start = tc.find("<");
		int end = tc.find(">");
		if (start >= end || start == 0)
			return;
		else
		{
			if (end == tc.length())
				end = tc.find(">::");
			if (end == tc.length())
				return;
		}
		std::string save = tc[end];
		std::string tok = tc[start - 1];
		for (int i = start; i < end + 1; ++i)
			tok += tc[i];
		for (int i = start; i < end + 1; ++i)
			tc.remove(start);
		if (save == ">::")
		{
			tok += tc[start + 1];
			tc.remove(start);
		}
		tc[start - 1] = tok;
		//std::cout << "\n  -- " << tc.show();
	}
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" , "each", "else", "else if" };
		for (int i = 0; i<8; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void removeInvocationParens(ITokCollection& tc)
	{
		int start = tc.find("(");
		int end = tc.find(")");
		if (start >= end || end == tc.length() || start == 0)
			return;
		if (isSpecialKeyWord(tc[start - 1]))
			return;
		for (int i = start; i < end + 1; ++i)
			tc.remove(start);
		//std::cout << "\n  -- " << tc.show();
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& in = *pTc;
		Scanner::SemiExp tc;
		for (int i = 0; i<in.length(); ++i)
			tc.push_back(in[i]);

		if (tc[tc.length() - 1] == ";" && tc.length() > 2)
		{
			//std::cout << "\n  ++ " << tc.show();
			removeInvocationParens(tc);
			condenseTemplateTypes(tc);

			// remove modifiers, comments, newlines, returns, and initializers

			Scanner::SemiExp se;
			for (int i = 0; i < tc.length(); ++i)
			{
				if (isModifier(tc[i]))
					continue;
				if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
					continue;
				if (tc[i] == "=" || tc[i] == ";")
					break;
				else
					se.push_back(tc[i]);
			}
			//std::cout << "\n  ** " << se.show();
			if (se.length() == 2)  // type & name, so declaration
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////
// Actions to find potential denpendency, looking up in the
// extend typetable
class addDeclDep : public IAction
{
private:
	Repository* p_Repos;
public:
	addDeclDep(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	std::string collectFile(std::vector<struct typedefi> typedefinfo)
	{
		std::string infile;
		for (auto typeif : typedefinfo)
		{
			infile = typeif.inFile;
		}
		return infile;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		std::map<std::string, std::vector<struct typedefi>> typetable_extend = p_Repos->getextendtable();
		for (int i = 0; i < tc.length(); i++)
		{
			std::map<std::string, std::vector<struct typedefi>>::iterator got = typetable_extend.find(tc[i]);
			if (got != typetable_extend.end())
				p_Repos->addDepend(collectFile(typetable_extend[tc[i]]));
		}
	}
};

///////////////////////////////////////////////////////////
// Rules to detect using of namespaces, in using namespace 
// form or in namespace:: form.
class UsingNameSpace : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		int len = tc.length();
		if (tc.find("using") < len && tc.find("namespace") < len)
		{
			doActions(pTc);
			return true;
		}
		if (tc.find("::") < len)
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

/////////////////////////////////////////////////////////
// Actions to add identified denpendencies to the vector
class addNSDep : public IAction
{
private:
	Repository* p_Repos;
public:
	addNSDep(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	std::string collectFile(std::vector<struct typedefi> typedefinfo)
	{
		std::string infile;
		for (auto typeif : typedefinfo)
		{
			infile = typeif.inFile;
		}
		return infile;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		int len = tc.length();
		std::string to_find;
		if (tc.find("using") < len && tc.find("namespace") < len)
		{
			int len_usingNS = tc.find("namespace");
			to_find = tc[len_usingNS+1];
		}
		if (tc.find("::") < len)
		{
			int len_NS = tc.find("::");
			to_find = tc[len_NS - 1];
		}
		std::map<std::string, std::vector<struct typedefi>> typetable_extend = p_Repos->getextendtable();
		std::map<std::string, std::vector<struct typedefi>>::iterator got = typetable_extend.find(to_find);
		p_Repos->addDepend(collectFile(typetable_extend[to_find]));
	}
};

///////////////////////////////////////////////////////////
// Rules to detect using of typedef dependencies.

class UsingTypeDef : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		int len = tc.length();
		// find "using" and "=", should be typedefine
		if (tc.find("using") < len && tc.find("=") < len)
		{
			doActions(pTc);
			return true;
		} 
		return true;
	}
};

///////////////////////////////////////////////////////////
// Actions to handle using of typedef dependencies.

class addUsingTypeDef : public IAction
{
private:
	Repository* p_Repos;
public:
	addUsingTypeDef(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	std::string collectFile(std::vector<struct typedefi> typedefinfo)
	{
		std::string infile;
		for (auto typeif : typedefinfo)
		{
			infile = typeif.inFile;
		}
		return infile;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		int len = tc.find("=");
		std::vector<std::string> to_find;
		for (int i = len; i < tc.length(); i++)
		{
			to_find.push_back(tc[i]);
		}
		std::map<std::string, std::vector<struct typedefi>> typetable_extend = p_Repos->getextendtable();
		std::map<std::string, std::vector<struct typedefi>>::iterator got;
		for (int i = 0; i < (int)to_find.size(); i++)
		{
			got = typetable_extend.find(to_find[i]);
			if (got != typetable_extend.end())
				p_Repos->addDepend(collectFile(typetable_extend[to_find[i]]));
		}
	}
};
#endif
