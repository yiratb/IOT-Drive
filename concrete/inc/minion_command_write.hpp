#ifndef ILRD_RD1556_MINION_COMMAND_WRITE_HPP
#define ILRD_RD1556_MINION_COMMAND_WRITE_HPP

#include "icommand.hpp"

class MinionCommandWrite : public ICommand
{
public:
    MinionCommandWrite() = default;
    ~MinionCommandWrite() = default;
    p_async_response Run(std::shared_ptr<IKeyTask> task_);
};

#endif //ILRD_RD1556_MINION_COMMAND_WRITE_HPP