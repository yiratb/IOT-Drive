#include <iostream>

#define ALLOWED_SINGLETON
#include "../inc/singleton.hpp"
#undef ALLOWED_SINGLETON

extern "C" void Foo()
{
    int *x = Singleton<int>::GetInstance();
    std::cout << "Address" << x << std::endl;
}