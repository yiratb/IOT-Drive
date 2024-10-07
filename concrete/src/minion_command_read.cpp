#include <unistd.h> //lseek, read

#include "minion_command_read.hpp"
#include "master_proxy.hpp"
#include "minion_utility.hpp"
#include "minion_key_task.hpp"
#include <fcntl.h>

ICommand::p_async_response MinionCommandRead::Run(std::shared_ptr<IKeyTask> task_)
{
    std::cout << "Start MinionCommandRead::Run" << std::endl;

    MinionUtility *minion_utility = Singleton<MinionUtility>::GetInstance();
    std::shared_ptr<MinionKeyTask> minion_key_task = std::dynamic_pointer_cast<MinionKeyTask>(task_);

    MasterProxy *master_proxy = Singleton<MasterProxy>::GetInstance();
    MinionUtility::Status status = MinionUtility::Status::SUCCESS;

    auto data = minion_key_task->GetData();

    int fd = minion_utility->GetFd();

    if (lseek(fd, data.m_from , SEEK_SET) == -1) 
    {
        std::cout << "MinionCommandRead lseek failed" << std::endl;
        master_proxy->SendResponseRead(MinionUtility::Status::FAIL, task_);
        return ICommand::p_async_response(nullptr, 0);
    }
    
    ssize_t bytes_read = read(fd, data.m_chunk, data.m_len);
    if (-1 == bytes_read) 
    {
        std::cout << "MinionCommandRead read failed" << std::endl;
        master_proxy->SendResponseRead(MinionUtility::Status::FAIL, task_);
        return ICommand::p_async_response(nullptr, 0);
    }

    master_proxy->SendResponseRead(status, task_);

    return ICommand::p_async_response(nullptr, 0);
}
