/****************************************************                                                                                              
 *  Written by: Yirat Romano                         
 *                                                  
 *  Reviewed by:                                
 *                                                  
 *  Completed on - XX/XX/24                         
 *                                                  
 ****************************************************/

/******************************* INCLUDES *************************************/

#include <iostream> //std
#include <thread>
#include <atomic>

#include "scheduler.hpp" 

/***************************** TYPEDEFS & STRUCTS *****************************/

class TaskHello : public Scheduler::ITask
{
public:
	void Execute() {std::cout << "Hello ";}
};

class TaskWorld : public Scheduler::ITask
{
public:
	void Execute() {std::cout << "World!" << std::endl;}
};

class TaskCount : public Scheduler::ITask
{
public:
	void Execute() {m_counter.fetch_add(1, std::memory_order_relaxed);}
	static std::atomic<int> m_counter;
};

std::atomic<int> TaskCount::m_counter{0};

/************************ STATIC FUNCTIONS DECLARATIONS ***********************/

/************************************ MAIN ************************************/

int main()
{
	Scheduler *scheduler = Singleton<Scheduler>::GetInstance();

	std::cout << "Success if printed \"Hello World!\" 4 times" << std::endl;

	scheduler->Add(std::make_shared<TaskWorld>(), std::chrono::milliseconds(2000));
	scheduler->Add(std::make_shared<TaskHello>(), std::chrono::milliseconds(1000));
	std::this_thread::sleep_for(std::chrono::seconds(3));

	scheduler->Add(std::make_shared<TaskWorld>(), std::chrono::milliseconds(1500));
	scheduler->Add(std::make_shared<TaskHello>(), std::chrono::milliseconds(1100));
	std::this_thread::sleep_for(std::chrono::seconds(2));
	
	scheduler->Add(std::make_shared<TaskWorld>(), std::chrono::milliseconds(2500));
	scheduler->Add(std::make_shared<TaskHello>(), std::chrono::milliseconds(1200));
	std::this_thread::sleep_for(std::chrono::seconds(3));
	
	scheduler->Add(std::make_shared<TaskHello>(), std::chrono::milliseconds(1000));
	scheduler->Add(std::make_shared<TaskWorld>(), std::chrono::milliseconds(1001));
	std::this_thread::sleep_for(std::chrono::seconds(10));

	int expected = 10000;
	for(int i = 0; i < expected; ++i)
	{
		scheduler->Add(std::make_shared<TaskCount>(), std::chrono::milliseconds(0));
	}

	std::this_thread::sleep_for(std::chrono::seconds(15));

	if (TaskCount::m_counter.load((std::memory_order_relaxed)) != expected)
	{
		std::cout << "Fail on task count " << TaskCount::m_counter << std::endl;
	}
	else
	{
		std::cout << "Success on task count" << std::endl;
	}
	

	return 0;
}


/*********************** STATIC FUNCTIONS DEFINITIONS ************************/

void PrintHello()
{
	std::cout << "Hello ";
}

void PrintWorld()
{
	std::cout << "World!" << std::endl;
}


