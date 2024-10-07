#ifndef ILRD_RD1556_COMMAND_FLUSH_HPP
#define ILRD_RD1556_COMMAND_FLUSH_HPP

#include <memory>
#include <iostream>

#include "icommand.hpp"
#include "ikey_task.hpp"
#include "nbd_key_task.hpp"

class NBDCommandFlush : public ICommand
{
public:
    explicit NBDCommandFlush() = default;
    ~NBDCommandFlush() = default;
    p_async_response Run(std::shared_ptr<IKeyTask> task_);
};

#endif //ILRD_RD1556_COMMAND_FLUSH_HPP