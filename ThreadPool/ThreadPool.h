#pragma once
///////////////////////////////////////////////////////////////////////
// ThreadPool.h - child thread processes enqueued work items         //
//                                                                   //
// Author: Tianhang Zhang , CSE687, Project3, S2016	                 //
//         tzhan116@syr.edu									         //
// Sourse: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016 //
///////////////////////////////////////////////////////////////////////
/*
* Create and handle multi thread operation.
*/

#include <thread>
#include <functional>
#include "BlockingQueue.h"

template<typename Result>
using WorkItem = std::function<Result()>;
#define NUMTHREAD  4

///////////////////////////////////////////////////////////////////////
// class to process work items
//

template<typename Result>
class ThreadPool
{
public:
	void start();
	void doWork(std::function<Result()>* pWi);
	//std::vector<Result> results();
	Result result();
	void wait();
	int NumResult() { return (int)_resultsQueue.size(); }
	~ThreadPool();
private:
	std::thread* _pThread;
	BlockingQueue<std::function<Result()>*> _workItemQueue;
	BlockingQueue<Result> _resultsQueue;
	std::vector<std::thread*> _pWorkingthr;
	//std::queue<std::thread*> _pIdlethr;
};
//----< wait for child thread to terminate >---------------------------

template<typename Result>
void ThreadPool<Result>::wait()
{
	//for (int i = 0; i < NUMTHREAD; i++)
	//{
	//	_pWorkingthr[i]->join();
	//}
	for (auto t : _pWorkingthr)
	{
		//if (t->joinable())
			t->join();
	}
}
//----< enqueue work item >--------------------------------------------

template<typename Result>
void ThreadPool<Result>::doWork(std::function<Result()> *pWi)
{
	_workItemQueue.enQ(pWi);
}
//----< retrieve results with blocking call >--------------------------

//template<typename Result>
//std::vector<Result> ThreadPool<Result>::results()
//{
//	std::vector<Result> results;
//	while (_resultsQueue.size() > 0)
//		results.push_back(_resultsQueue.deQ());
//	return results;
//}

template<typename Result>
Result ThreadPool<Result>::result()
{
	return _resultsQueue.deQ();
}

//----< start child thread that dequeus work items >-------------------

template<typename Result>
void ThreadPool<Result>::start()
{
	static std::mutex m;
	std::function<void()> threadProc =
		[&]() {
		while (true)
		{
			std::function<Result()>* pWi = _workItemQueue.deQ();
			if (pWi == nullptr)
			{
				m.lock();
				std::cout << "\n  shutting down thread# " << std::this_thread::get_id() ;
				m.unlock();
				return;
			}
			Result result = (*pWi)();
			//_pIdlethr.push();
			_resultsQueue.enQ(result);
		}
	};
	// creating the pool of working threads
	// push all the new threads into the idle list for future use
	for (int i = 0; i < NUMTHREAD; i++)
	{
		_pWorkingthr.push_back(new std::thread(threadProc));
		//_pIdlethr.push(_pWorkingthr[i]);
	}
	std::cout << "Working with " << _pWorkingthr.size() << "  threads.\n\n";
}
//----< clean up heap >------------------------------------------------

template<typename Result>
ThreadPool<Result>::~ThreadPool()
{
	for each (std::thread* thr in _pWorkingthr)
	{
		delete thr;
	}
}


