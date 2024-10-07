#include "nbd_command_trim.hpp"
#include "nbd_proxy.hpp"
#include "logger.hpp"

ICommand::p_async_response NBDCommandTrim::Run(std::shared_ptr<IKeyTask> task_)
{
    std::shared_ptr<NBDKeyTask> task = std::dynamic_pointer_cast<NBDKeyTask>(task_);
    Logger *logger = Singleton<Logger, const std::string&>::GetInstance("logger_cpf");
    logger->Log("NBD Command Trim Run", Logger::INFO);

    NbdProxy *nbd_proxy = Singleton<NbdProxy>::GetInstance();

    nbd_proxy->SendResponse(task, nullptr);

    return ICommand::p_async_response(nullptr, 0);
}