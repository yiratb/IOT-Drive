/****************************************************                                                                                              
 *  Written by: Yurat Romano                          
 *                                                  
 *  Reviewed by:                                
 *                                                  
 *  Completed on - XX/XX/24                         
 *                                                  
 ****************************************************/

/******************************* INCLUDES *************************************/

#include <iostream> /* std */
#include <cstdlib> /* new */

#include "factory.hpp" /* factroy */

/***************************** TYPEDEFS & STRUCTS *****************************/
class A
{
public:
	A(size_t x): m_ax(x) {}
	virtual void Print() {std::cout << m_ax << ", class A\n" << std::endl;}
	virtual ~A() = default;
	size_t m_ax;	
};

class B: public A
{
public:
	B(size_t x): A(x) {} 
	virtual void Print() {}
	virtual ~B() = default;
};

/************************ STATIC FUNCTIONS DECLARATIONS ***********************/

/************************************ MAIN ************************************/

int main()
{
	Factory<A, int , size_t> *f = Singleton<Factory<A, int , size_t>>::GetInstance();

	f->Add(1, [](size_t x) { return std::make_unique<A>(x); });
    f->Add(2, [](size_t x) { return std::make_unique<B>(x); });
	
	auto a = f->Create(1, 50);
	auto b = f->Create(2, 123);
	//auto c = f.Create(8, 789987, 1.321);

	a->Print();
	b->Print();

	return 0;
}


/*********************** STATIC FUNCTIONS DEFINITIONS ************************/