#ifndef ILRD_RD1556_COMMAND_READ_HPP
#define ILRD_RD1556_COMMAND_READ_HPP

#include "icommand.hpp"
#include "ikey_task.hpp"

class NBDCommandRead : public ICommand
{
public:
    explicit NBDCommandRead() = default;
    ~NBDCommandRead() = default;
    NBDCommandRead(const NBDCommandRead& other_) = delete;
    NBDCommandRead& operator=(const NBDCommandRead& other_) = delete;
    p_async_response Run(std::shared_ptr<IKeyTask> task_);
};

#endif //ILRD_RD1556_COMMAND_READ_HPP