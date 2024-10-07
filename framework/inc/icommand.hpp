#ifndef ILRD_RD1556_ICOMMAND_HPP
#define ILRD_RD1556_ICOMMAND_HPP

#include <functional> //std::function
#include <memory> //std::shared_ptr

#include "ikey_task.hpp"//IKeyTask
#include "chrono" //std::chrono::milliseconds

class ICommand // for command to run
{
public:
    using p_async_response = std::pair<std::function<bool()>, std::chrono::milliseconds>;

    virtual ~ICommand() = default;
    virtual p_async_response Run(std::shared_ptr<IKeyTask> task_) = 0;
};

#endif //ILRD_RD1556_ICOMMAND_HPP