#include <unistd.h> //lseek, write

#include "minion_command_write.hpp"
#include "master_proxy.hpp"
#include "minion_utility.hpp"
#include "minion_key_task.hpp"

ICommand::p_async_response MinionCommandWrite::Run(std::shared_ptr<IKeyTask> task_)
{    
    std::cout << "Start MinionCommandWrite::Run" << std::endl;

    MinionUtility *minion_utility = Singleton<MinionUtility>::GetInstance();
    std::shared_ptr<MinionKeyTask> minion_key_task = std::dynamic_pointer_cast<MinionKeyTask>(task_);
    MinionUtility::Status status = MinionUtility::Status::SUCCESS;

    int fd = minion_utility->GetFd();
    if (lseek(fd, minion_key_task->GetData().m_from, SEEK_SET) == -1) 
    {
        status = MinionUtility::Status::FAIL;
    }

    ssize_t bytesW_written = write(fd, minion_key_task->GetData().m_chunk,
                                     minion_key_task->GetData().m_len);
    if (-1 == bytesW_written) 
    {
        status = MinionUtility::Status::FAIL;
    }

    MasterProxy *master_proxy = Singleton<MasterProxy>::GetInstance();
    master_proxy->SendResponseWrite(status, task_);

    return ICommand::p_async_response(nullptr, 0);
}