#include "nbd_command_disc.hpp"
#include "logger.hpp"

ICommand::p_async_response NBDCommandDisc::Run(std::shared_ptr<IKeyTask> task_)
{
    (void)task_;

    Logger *logger = Singleton<Logger, const std::string&>::GetInstance("logger_cpf");
    logger->Log("NBD Command Disc Run", Logger::INFO);

    return ICommand::p_async_response(nullptr, 0);
}
