#include "nbd_command_flush.hpp"
#include "nbd_proxy.hpp"
#include "logger.hpp"

ICommand::p_async_response NBDCommandFlush::Run(std::shared_ptr<IKeyTask> task_)
{
    std::shared_ptr<NBDKeyTask> task = std::dynamic_pointer_cast<NBDKeyTask>(task_);
    Logger *logger = Singleton<Logger, const std::string&>::GetInstance("logger_cpf");
    logger->Log("NBD Command Flush Run", Logger::INFO);

    NbdProxy *nbd_proxy = Singleton<NbdProxy>::GetInstance();

    nbd_proxy->SendResponse(task, nullptr);

    return ICommand::p_async_response(nullptr, 0);
}