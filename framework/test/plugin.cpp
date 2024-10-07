#include <iostream>

#include "../inc/factory.hpp"
#include "plugins_test.hpp"

class Derived : public Base
{
public:
    Derived() = default;
    ~Derived() = default;
    void Print();
    
};

void Derived::Print()
{
    std::cout << "Print Derived" << std::endl;
}

std::shared_ptr<Base> crtC()
{
    return std::make_shared<Derived>();
}

__attribute__((constructor)) void Ctor()
{
    Factory<Base, size_t> *f = Singleton<Factory<Base, size_t>>::GetInstance();
    auto func = crtC;
    std::cout << "Address in ctor" << f << std::endl;
    f->Add(3, func);
    std::cout << "This runs before main!\n";
}


__attribute__((destructor)) void Dtor()
{
    Factory<Base, size_t> *f = Singleton<Factory<Base, size_t>>::GetInstance();
    std::cout << "Address in dtor" << f << std::endl;
    f->Remove(3);
    std::cout << "dll closed" << std::endl;
}
