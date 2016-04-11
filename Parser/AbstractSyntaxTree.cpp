///////////////////////////////////////////////////////////////////////////////
// AbstractSyntaxTree.cpp - implements abstractsyntaxtree to hold the whole  //
// structure of program in a tree.											 //
// ver 2.0                                                                   //
// Language:      Visual C++ 2015, SP1                                       //
// Application:   Code Analysis Research                                     //
// Author:        Tianhang Zhang, Syracuse University						 //
//                (315) 383-3757, tzhan116@syr.edu		                     //
///////////////////////////////////////////////////////////////////////////////

//----< test stub >------------------------------------------------------

#include "AbstractSyntaxTree.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

//Node::Node(std::string _type, std::string _name, int _startLineCount, int _endLineCount, std::vector<Node*> _children)
Node::Node(std::string _type, std::string _name, int _LineCount, std::vector<Node*> _children)
{
	type = _type;
	name = _name;
	//startLineCount = _startLineCount;
	//endLineCount = _endLineCount;
	LineCount = _LineCount;
	children = _children;
}

std::string Node::getType()
{
	return type;
}

std::string Node::getName()
{
	return name;
}

std::vector<Node*> Node::getChildren()
{
	return children;
}

int Node::getChildrenNum()
{
	return (int)children.size();
}

int Node::getLineCount()
{
	if (startLineCount && endLineCount)
		return (int)startLineCount - endLineCount;
	return (int)LineCount;
}

void Node::addChildren(Node* _node)
{

	children.push_back(_node);
}

void Node::setStartLine(int startline)
{
	startLineCount = startline;
}

void Node::setEndLine(int endline)
{
	endLineCount = endline;
}

int Node::getHeight(Node* _node)
{
	static int height = 1;
	if (_node->getChildrenNum() != 0)
		height++;
	else
		return height;
	std::vector<int> h;
	for each (Node* child in _node->getChildren())
	{
		h.push_back(getHeight(child));
	}
	std::sort(h.begin(), h.end());
	//height += h[h.size() - 1];
	return height;
}

void Node::showNode()
{
	std::cout << "-----------------------------------------------" << "\n";
	std::cout << "Type:" << "    " << type << "\n";
	std::cout << "Name:" << "    " << name << "\n";
	std::cout << "Start From Line:" << "    " << endLineCount << "\n";
	std::cout << "End At Line:" << "    " << LineCount << "\n";
	std::cout << "Total Lines: " << "    " << LineCount - endLineCount << "\n";
	std::cout << "Children number: " << "    " << children.size() << "\n";
}

int AbstractSyntaxTree::getTreeSize(Node* _node)
{
	static int nodecount = 1;
	nodecount += _node->getChildrenNum();
	for each (Node* child in _node->getChildren())
	{
		nodecount += getTreeSize(child);
	}
	return nodecount;
}

int AbstractSyntaxTree::getHeight(Node* _node)
{
	static int height = 1;
	if (_node->getChildrenNum() != 0)
		height++;
	std::vector<int> h;
	for each (Node* child in _node->getChildren())
	{
		h.push_back(getHeight(child));
	}
	std::sort(h.begin(), h.end());
	height += h[h.size() - 1];
	return height;
}

void AbstractSyntaxTree::showAST(std::string filename)
{
	Node* cur_node;
	std::cout << "---------------------------------------------------" << "\n";
	std::cout << "-----------------Showing Nodes in AST--------------" << "\n";
	std::cout << " Generated from " << "    " << filename << "\n";
	std::cout << "-----------------------------------------------" << "\n";
	for (int i = 1; i <= nodecount; i++)
	{
		cur_node = allNodes[i];
		//std::cout << "-----------------------------------------------" << "\n";
		//std::cout << "Node#" << "   " << i << "   " << ":" << "\n";
		//std::cout << "Type:" << "    " << cur_node->getType() << "\n";
		//std::cout << "Name:" << "    " << cur_node->getName() << "\n";
		//std::cout << "Start From Line:" << "    " << cur_node->getLineCount() << "\n";
		//std::cout << "End At Line:" << "    " << cur_node->getendLineCount() << "\n";
		//std::cout << "Total Lines: " << "    " << cur_node->getendLineCount() - cur_node->getLineCount() << "\n";
		//std::cout << "Children number: " << "    " << cur_node->getChildrenNum() << "\n";
		for (int j = 0; j < cur_node->getDepth(); j++)
			std::cout << " ";
		std::cout << " -- ( Node# " << i << "  :  " << cur_node->getType() << " , " << cur_node->getName() << " , " << " start:" << cur_node->getLineCount() << "  end: " << cur_node->getendLineCount() << " total: " << cur_node->getendLineCount() - cur_node->getLineCount() << " )" << "\n";
	}
	std::cout << "-----------------------------------------------" << "\n";
}

void AbstractSyntaxTree::showAST()
{
	Node* cur_node;
	std::cout << "-----------------------------------------------" << "\n";
	for (int i = 1; i <= nodecount; i++)
	{
		cur_node = allNodes[i];
		//std::cout << "Node#" << "   " << i << "   " << ":" << "\n";
		//std::cout << "Type:" << "    " << cur_node->getType() << "\n";
		//std::cout << "Name:" << "    " << cur_node->getName() << "\n";
		//std::cout << "Start From Line:" << "    " << cur_node->getLineCount() << "\n";
		//std::cout << "End At Line:" << "    " << cur_node->getendLineCount() << "\n";
		//std::cout << "Total Lines: " << "    " << cur_node->getendLineCount() - cur_node->getLineCount() << "\n";
		//std::cout << "Children number: " << "    " << cur_node->getChildrenNum() << "\n";
		for (int j = 0; j < cur_node->getDepth(); j++)
			std::cout << " ";
		std::cout << " -- ( Node# " << i << "  :  " << cur_node->getType() << " , " << cur_node->getName() << " , " << " start:" << cur_node->getLineCount() << "  end: " << cur_node->getendLineCount() << " total: " << cur_node->getendLineCount() - cur_node->getLineCount() << " )" << "\n";
	}
	std::cout << "-----------------------------------------------" << "\n";
}

