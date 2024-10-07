#include <iostream>
#include <stdio.h>
#include <dlfcn.h>

/* #include "dynamic_library.hpp" */
#define ALLOWED_SINGLETON
#include "../inc/singleton.hpp"
#undef ALLOWED_SINGLETON

/* class MyClass 
{
public:
    MyClass(int value) : m_value(value) {}
    void display() const { std::cout << "Value: " << m_value << std::endl; }
private:
    int m_value;
};
 */
int main() 
{
    int *x = Singleton<int>::GetInstance();
    std::cout << "Address" << x << std::endl;

    /* void *lib = dlopen("./libdynamic1.so", RTLD_LAZY);
    if (lib == NULL) 
    {
        fprintf(stderr, "Error: %s\n", dlerror());
        return 1;
    }
    
    void (*Foo)() = reinterpret_cast<void (*)()>(dlsym(lib, "Foo"));
    if (Foo == NULL) 
    {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(lib);
        return 1;
    }

    Foo(); */

    return 0;
}
