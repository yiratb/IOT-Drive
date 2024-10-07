
#ifndef ILRD_RD1556_Singleton_HPP
#define ILRD_RD1556_Singleton_HPP

#include <mutex> /*std::mutex*/
#include <cstdlib> //atexit
#include <iostream>

/*
- class T must have operator new
- in order do use GetInstance, you must define ALLOWED_SINGLETON before the header
- note that the usage of GetInstance enforces an atexit cleanup function
*/

template <typename T, typename... ARGS>
class Singleton
{
public:
    Singleton() = delete;
    ~Singleton() = delete;
    Singleton(const Singleton& other_) = delete;
    Singleton& operator=(const Singleton& other_) = delete;
    static T* GetInstance(ARGS... args);

private:
    static std::mutex m_mutex;
    static volatile T* m_instance;
};

template <typename T, typename... ARGS>
std::mutex Singleton<T, ARGS...>::m_mutex;

template <typename T, typename... ARGS>
volatile T* Singleton<T, ARGS...>::m_instance = nullptr;

#ifdef ALLOWED_SINGLETON
template <typename T, typename... ARGS>
T* Singleton<T, ARGS...>::GetInstance(ARGS... args)
{
    if (nullptr == m_instance)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (nullptr == m_instance) 
        {
            volatile T *tmp = new T(args...);
            m_instance = tmp;
            atexit([]{delete m_instance; m_instance = nullptr;});
        }
    }
    return const_cast<T *>(Singleton<T, ARGS...>::m_instance);
}
#endif /*ALLOWED_SINGLETON*/

#endif // ILRD_RD1556_Singleton_HPP

