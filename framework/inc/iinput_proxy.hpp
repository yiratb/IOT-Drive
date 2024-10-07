#ifndef ILRD_RD1556_IINPUTPROXY_HPP
#define ILRD_RD1556_IINPUTPROXY_HPP

#include <memory> //std::shared_pointer

#include "ikey_task.hpp"//IKeyTask

class IInputProxy // for input parsing
{
public:
    virtual std::shared_ptr<IKeyTask> GetTask() = 0;
    virtual ~IInputProxy() = default;
};

#endif //ILRD_RD1556_IINPUTPROXY_HPP