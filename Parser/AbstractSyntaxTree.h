#pragma once
///////////////////////////////////////////////////////////////////////////////
// AbstractSyntaxTree.h - Declaration of AbstractSyntaxTree.			     //
// ver 2.0                                                                   //
// Language:      Visual C++ 2015                                            //
// Application:   Code Analysis Research                                     //
// Author:        Tianhang Zhang, Syracuse University						 //
//                (315) 383-3757, tzhan116@syr.edu		                     //
///////////////////////////////////////////////////////////////////////////////

//----< test stub >------------------------------------------------------

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <map>

// Class to handle and hold informations for scopes
class Node
{
private:
	std::string type;
	std::string name;
	int LineCount;
	int startLineCount = 0;
	int endLineCount = 0;
	std::vector<Node*> children;
	int depth;
public:
	Node() {};
	Node(std::string _type, std::string _name, int _startLineCount, int _endLineCount, std::vector<Node*> _children);
	Node(std::string _type, std::string _name, int _LineCount, std::vector<Node*> _children);
	std::string getName();
	std::string getType();
	std::vector<Node*> getChildren();
	int getChildrenNum();
	int getLineCount();
	void addChildren(Node* _node);
	void setStartLine(int startline);
	void setEndLine(int endline);
	int getstartLineCount() { return startLineCount; }
	int getendLineCount() { return endLineCount; }
	int getHeight(Node* _node);
	void showNode();
	void setDepth(int dep) { depth = dep; }
	int getDepth() { return depth;  }
};


// class to hold the whole syntax tree
class AbstractSyntaxTree 
{
private:
	Node* root;
	int nodecount = 0;
	std::map<int, Node*> allNodes;
public:
	AbstractSyntaxTree(Node* _node) { 
		root = _node;  nodecount++; 
		allNodes.insert(std::pair<int, Node*>(nodecount, _node));
	}
	int getTreeSize(Node* _node);
	int getHeight(Node* _node);
	void addNode(Node* _node) { 
		nodecount++; 
		allNodes.insert(std::pair<int, Node*>(nodecount, _node)); 
	}
	Node* getRoot() { return root;	}
	void showAST(std::string filename);
	void showAST();
};
