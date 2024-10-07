/****************************************************                                                                                              
 *  Written by: Itamar Bloch                           
 *                                                  
 *  Reviewed by: Adi Telm                               
 *                                                  
 *  Completed on - XX/XX/24                         
 *                                                  
 ****************************************************/

/******************************* INCLUDES *************************************/

#include <iostream> //std
#include <unistd.h> //sleep

#include "async_injection.hpp" //

/***************************** TYPEDEFS & STRUCTS *****************************/
int const COUNT = 10000;
/************************ STATIC FUNCTIONS DECLARATIONS ***********************/
bool PrintHello();
bool PrintWorld();
bool Success();
/************************************ MAIN ************************************/

int main()
{
	AsyncInjection *async_injection1 = new AsyncInjection(PrintWorld, std::chrono::milliseconds(1000));
	AsyncInjection *async_injection2 = new AsyncInjection(PrintHello, std::chrono::milliseconds(950));

	std::cout << "Should Print 5 HelloWorld\n";
	for (int i = 0; i < 10; ++i)
	{
		sleep(1);
	}

	AsyncInjection *async_injection3 = new AsyncInjection(Success, std::chrono::milliseconds(0));
	for (int i = 0; i < 3; ++i)
	{
		sleep(1);
	}
    sleep(6);
    (void)async_injection3;
    (void)async_injection2;
    (void)async_injection1;

	return 0;
}


/*********************** STATIC FUNCTIONS DEFINITIONS ************************/
bool PrintHello()
{
	static int count = 0;

	std::cout << "Hello";

	++count;
	return (count > 4);
}

bool PrintWorld()
{
	static int count = 0;

	std::cout << "World: " << count << "\n";

	++count;
	return (count > 4);
}

bool Success()
{
	static int count = 0;

	++count;
	if (count == COUNT)
	{
		std::cout << "PASS, Count Test\n";
	}

	return (count > (COUNT - 1));
}
