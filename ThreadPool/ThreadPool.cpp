///////////////////////////////////////////////////////////////////////
// ThreadPool.cpp - child thread processes enqueued work items       //
//                                                                   //
// Author: Tianhang Zhang , CSE687, Project3, S2016	                 //
//         tzhan116@syr.edu									         //
// Sourse: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016 //
///////////////////////////////////////////////////////////////////////
/*
* Create and handle multi thread operation.
*/

#include <string>
#include <iostream>
#include <conio.h>
#include "ThreadPool.h"
#include "../Utilities/Utilities.h"

//----< demonstrate ProcessWorkItem class >----------------------------

using Util = Utilities::StringHelper;
using WorkResult = std::string;

#ifdef TEST_THREADPOOL
int main()
{
	Util::Title("Enqueued Work Items");

	std::cout << "\n  main thread id = " << std::this_thread::get_id();

	ThreadPool<WorkResult> processor;
	processor.start();

	// define 1st work item

	WorkItem<WorkResult> wi1 = []() {
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		return "Hello from wi1";
	};

	processor.doWork(&wi1);

	// define 2nd work item

	WorkItem<WorkResult> wi2 = []()
	{
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		size_t sum = 0;
		for (size_t i = 0; i < 100000; ++i)
			sum += i;
		return "wi2 result = " + Utilities::Converter<size_t>::toString(sum);
	};

	processor.doWork(&wi2);

	// define 3rd work item

	WorkItem<WorkResult> wi3 = []()
	{
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		size_t sum = 0;
		for (size_t i = 0; i < 100000; ++i)
			sum += i;
		return "wi3 result = " + Utilities::Converter<size_t>::toString(sum);
	};

	processor.doWork(&wi3);

	// define 4th work item

	WorkItem<WorkResult> wi4 = []()
	{
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		size_t sum = 0;
		for (size_t i = 0; i < 100000; ++i)
			sum += i;
		return "wi4 result = " + Utilities::Converter<size_t>::toString(sum);
	};

	processor.doWork(&wi4);

	// define 5th work item

	WorkItem<WorkResult> wi5 = []()
	{
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		size_t sum = 0;
		for (size_t i = 0; i < 100000; ++i)
			sum += i;
		return "wi5 result = " + Utilities::Converter<size_t>::toString(sum);
	};

	processor.doWork(&wi5);

	// define 6th work item

	WorkItem<WorkResult> wi6 = []()
	{
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		size_t sum = 0;
		for (size_t i = 0; i < 100000; ++i)
			sum += i;
		return "wi6 result = " + Utilities::Converter<size_t>::toString(sum);
	};

	processor.doWork(&wi6);

	// the following calls to result() block until results are enqueued

	std::cout << "\n  " << processor.result();
	std::cout << "\n  " << processor.result();
	std::cout << "\n  " << processor.result();
	std::cout << "\n  " << processor.result();
	std::cout << "\n  " << processor.result();
	std::cout << "\n  " << processor.result();
	processor.doWork(nullptr);
	processor.doWork(nullptr);
	processor.doWork(nullptr);
	processor.doWork(nullptr);

	// wait for child thread to complete

	processor.wait();
	std::cout << "\n\n";
}
#endif