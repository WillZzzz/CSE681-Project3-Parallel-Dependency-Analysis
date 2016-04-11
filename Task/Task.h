#pragma once
///////////////////////////////////////////////////////////////////////
// Task.h - Wrapper around ThreadPool that uses the service provided //
//          by ThreadPool and prepare workitems for ThreadPool       //
//																	 //	
// Tianhang Zhang, CSE687 - Object Oriented Design, Spring 2016      //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
*Provides a Task class that executes a specified callable object on a 
*ThreadPool thread, using services of the ThreadPool package.
*/

#include <thread>
#include <functional>
#include "../ThreadPool/ThreadPool.h"
#include "../SemiExp/itokcollection.h"
#include "../FileMgr/FileMgr.h"
#include "../Tokenizer/Tokenizer.h"
#include <Windows.h>

template<typename Result>
using WorkItem = std::function<Result()>;

///////////////////////////////////////////////////////////////////////
// class to wrap the ThreadPool package and provide callable objects
//
template<typename Result>
class Task
{
public:
	Task() {  NumWorks = 0; }
	void addWork(WorkItem<Result>* pWi);
	void startwork() { TP.start(); }
	~Task() { TP.~ThreadPool(); }
	void join() { TP.wait(); }
	Result getResult();
	int NumResult() { return TP.NumResult(); }
	int NumWork() { return NumWorks; }
	//ThreadPool<Result>* getThreadPool() { return &TP; }
private:
	int NumWorks;
	ThreadPool<Result> TP;
};

template<typename Result>
void Task<Result>::addWork(WorkItem<Result>* pWi)
{
	TP.doWork(pWi);
	NumWorks++;
	Sleep(500);
}

template<typename Result>
Result Task<Result>::getResult()
{
	return TP.result();
}