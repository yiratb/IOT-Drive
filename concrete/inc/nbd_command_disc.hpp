#ifndef ILRD_RD1556_COMMAND_DISC_HPP
#define ILRD_RD1556_COMMAND_DISC_HPP

#include <memory>
#include <iostream>

#include "icommand.hpp"
#include "ikey_task.hpp"
#include "nbd_key_task.hpp"

class NBDCommandDisc : public ICommand
{
public:
    explicit NBDCommandDisc() = default;
    ~NBDCommandDisc() = default;
    NBDCommandDisc(const NBDCommandDisc& other_) = delete;
    NBDCommandDisc& operator=(const NBDCommandDisc& other_) = delete;
    p_async_response Run(std::shared_ptr<IKeyTask> task_);
};

#endif //ILRD_RD1556_COMMAND_DISC_HPP