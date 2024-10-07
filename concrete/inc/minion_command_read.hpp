#ifndef ILRD_RD1556_MINION_COMMAND_READ_HPP
#define ILRD_RD1556_MINION_COMMAND_READ_HPP

#include "icommand.hpp"

class MinionCommandRead : public ICommand
{
public:
    MinionCommandRead() = default;
    ~MinionCommandRead() = default;
    p_async_response Run(std::shared_ptr<IKeyTask> task_);
};

#endif //ILRD_RD1556_MINION_COMMAND_READ_HPP