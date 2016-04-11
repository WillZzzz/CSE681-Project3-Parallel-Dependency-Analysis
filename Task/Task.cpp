///////////////////////////////////////////////////////////////////////
// Task.cpp - Wrapper around ThreadPool that uses the service provided//
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
#include "Task.h"
#include "../Utilities/Utilities.h"

#ifdef TEST_TASK

using Util = Utilities::StringHelper;
using WorkResult = std::string;

void main()
{
	std::cout << "Testing Task package" << "\n";
	Util::Title("Enqueued Work Items");

	std::cout << "\n  main thread id = " << std::this_thread::get_id();

	Task<WorkResult>* task = new Task<WorkResult>();
	//ThreadPool<WorkResult> TP = task 
	
	// define 1st work item

	WorkItem<WorkResult> wi1 = []() {
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		return "Hello from wi1";
	};

	task->addWork(&wi1);

	// define 2nd work item

	WorkItem<WorkResult> wi2 = []()
	{
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		size_t sum = 0;
		for (size_t i = 0; i < 100000; ++i)
			sum += i;
		return "wi2 result = " + Utilities::Converter<size_t>::toString(sum);
	};

	task->addWork(&wi2);

	// define 3rd work item

	WorkItem<WorkResult> wi3 = []()
	{
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		size_t sum = 0;
		for (size_t i = 0; i < 100000; ++i)
			sum += i;
		return "wi3 result = " + Utilities::Converter<size_t>::toString(sum);
	};

	task->addWork(&wi3);

	// define 4th work item

	WorkItem<WorkResult> wi4 = []()
	{
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		size_t sum = 0;
		for (size_t i = 0; i < 100000; ++i)
			sum += i;
		return "wi4 result = " + Utilities::Converter<size_t>::toString(sum);
	};

	task->addWork(&wi4);

	// define 5th work item

	WorkItem<WorkResult> wi5 = []()
	{
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		size_t sum = 0;
		for (size_t i = 0; i < 100000; ++i)
			sum += i;
		return "wi5 result = " + Utilities::Converter<size_t>::toString(sum);
	};

	task->addWork(&wi5);

	// define 6th work item

	WorkItem<WorkResult> wi6 = []()
	{
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		size_t sum = 0;
		for (size_t i = 0; i < 100000; ++i)
			sum += i;
		return "wi6 result = " + Utilities::Converter<size_t>::toString(sum);
	};

	task->addWork(&wi6);

	// the following calls to result() block until results are enqueued

	std::cout << "\n  " << task->getResult();
	std::cout << "\n  " << task->getResult();
	std::cout << "\n  " << task->getResult();
	std::cout << "\n  " << task->getResult();
	std::cout << "\n  " << task->getResult();
	std::cout << "\n  " << task->getResult();
	task->addWork(nullptr);
	task->addWork(nullptr);
	task->addWork(nullptr);
	task->addWork(nullptr);

	// wait for child thread to complete
	task->join();
	std::cout << "\n\n";
}
#endif