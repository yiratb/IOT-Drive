/****************************************************                                                                                              
 *  Written by: Itamar Bloch                           
 *                                                  
 *  Reviewed by:                                
 *                                                  
 *  Completed on - XX/XX/24                         
 *                                                  
 ****************************************************/

/******************************* INCLUDES *************************************/

#include <iostream> /* std */
#include <cstdlib> /* new */
#include <unistd.h>

#include "thread_pool.hpp" /* thread_pool */

/***************************** TYPEDEFS & STRUCTS *****************************/
const char *PASS = "PASSED";
const char *FAIL = "FAILED";

class PrintTask: public ThreadPool::Task
{
public:	
	void Run() { std::cout << "PrintTask\n"; }
};

/************************ STATIC FUNCTIONS DECLARATIONS ***********************/
static const char *TestFlow1();
static const char *TestFlow2();
/************************************ MAIN ************************************/

int main()
{
	std::cout<< "TestFlow1: " << TestFlow1() << "\n";
    std::cout<< "TestFlow2: " << TestFlow2() << "\n";
	// std::cout<< "TestFlow3: " << TestFlow3() << "\n";
	// std::cout<< "TestFlow4: " << TestFlow4() << "\n";
	// std::cout<< "TestFlow5: " << TestFlow5() << "\n";

	return 0;
}


/*********************** STATIC FUNCTIONS DEFINITIONS ************************/
static const char *TestFlow1()
{
	const size_t num_of_task = 5;
	ThreadPool *tp = Singleton<ThreadPool, size_t>::GetInstance(num_of_task);
	std::shared_ptr<PrintTask> print_tasks(new PrintTask());

	tp->Pause();
	for (size_t i = 0; i < num_of_task; ++i)
	{
		tp->Add(print_tasks, ThreadPool::LOW);
	}
	sleep(5);
	tp->Resume();
	sleep(1);
	tp->Stop();
	for (size_t i = 0; i < num_of_task; ++i)
	{
		tp->Add(print_tasks, ThreadPool::LOW);
	}
	tp->Resume();

	return PASS;
}

static const char *TestFlow2()
{
	const size_t num_of_task = 16;
	ThreadPool *tp = Singleton<ThreadPool, size_t>::GetInstance(num_of_task);
	std::shared_ptr<PrintTask> print_tasks(new PrintTask());

	tp->Pause();
	for (size_t i = 0; i < num_of_task; ++i)
	{
		tp->Add(print_tasks, ThreadPool::LOW);
	}
	/* tp->SetNumOfThreads(30);
	sleep(2); */
	tp->SetNumOfThreads(3);
	tp->Stop();

	return PASS;
}
