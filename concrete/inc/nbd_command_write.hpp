#ifndef ILRD_RD1556_COMMAND_WRITE_HPP
#define ILRD_RD1556_COMMAND_WRITE_HPP

#include <memory> //std::shared_ptr

#include "icommand.hpp"
#include "ikey_task.hpp"

class NBDCommandWrite : public ICommand
{
public:
    explicit NBDCommandWrite() = default;
    ~NBDCommandWrite() = default;
    p_async_response Run(std::shared_ptr<IKeyTask> task_);
};

#endif //ILRD_RD1556_COMMAND_WRITE_HPP