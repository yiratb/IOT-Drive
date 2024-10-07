#ifndef ILRD_RD1556_FACTORY_HPP
#define ILRD_RD1556_FACTORY_HPP

#include <functional> // std::function
#include <unordered_map> // std::unordered map
#include <memory> // std::shared_ptr

#define ALLOWED_SINGLETON
#include "singleton.hpp" //Singleton
#undef ALLOWED_SINGLETON

/*
- Initialize the Factory by creating an instance through the Singleton pattern.
- Use the Add method to register a creation function for a specific key.
- Use the Remove method to unregister a creation function for a specific key.
- Use the Create method to instantiate an object based on the registered creation function for a specific key.
- Ensure the factory uses std::shared_ptr to manage object lifetimes.
- Refer to the singleton.hpp documentation for Singleton usage details.
    */

template <typename BASE, typename KEY, typename... ARGS>
class Factory
{
public:
    ~Factory() = default;
    Factory(const Factory& other_) = delete;
    Factory& operator=(const Factory& other_) = delete;
    void Add(const KEY key_, std::function<std::shared_ptr<BASE>(ARGS...)> func_);
    inline void Remove(const KEY key_);
    std::shared_ptr<BASE> Create(KEY key_, ARGS... args_) const;

private:
    explicit Factory() = default;
    friend Singleton<Factory>;
    std::unordered_map < KEY, std::function<std::shared_ptr<BASE>(ARGS...)>> m_ctors;
};

template <typename BASE, typename KEY, typename... ARGS>
inline void Factory<BASE, KEY, ARGS...>::Add
        (const KEY key_, std::function<std::shared_ptr<BASE>(ARGS...)> func_)
{
    m_ctors[key_] = func_;
}

template <typename BASE, typename KEY, typename... ARGS>
inline void Factory<BASE, KEY, ARGS...>::Remove(const KEY key_)
{
    m_ctors.erase(key_);
}

template <typename BASE, typename KEY, typename... ARGS>
inline std::shared_ptr<BASE> Factory<BASE, KEY, ARGS...>::Create
                                                (KEY key_, ARGS... args_) const
{
    auto iter = m_ctors.find(key_);

    if (iter != m_ctors.end())
    {
        return iter->second(std::forward<ARGS>(args_)...);
    }
    
    throw std::runtime_error("Key not found in map\n");
}

#endif // ILRD_RD1556_FACTORY_HPP