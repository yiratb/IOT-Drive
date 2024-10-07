#include "nbd_command_read.hpp"
#include "nbd_key_task.hpp"
#include "response_manager.hpp"
#include "raid_manager.hpp"
#include "nbd_proxy.hpp"
#include "minion_utility.hpp"
#include "logger.hpp"
#include <iostream>
using namespace std;

NBDCommandRead::p_async_response NBDCommandRead::Run(std::shared_ptr<IKeyTask> task_)
{
    // 1. Get args:
    std::shared_ptr<NBDKeyTask> task = std::dynamic_pointer_cast<NBDKeyTask>(task_);

    Logger *logger = Singleton<Logger, const std::string&>::GetInstance("logger_cpf");
    logger->Log("NBD Command Read Run", Logger::INFO);

    // 2. Get Minion Proxy and offset:
    RaidManager::p_minion_proxy minion_and_offest = 
            Singleton<RaidManager>::GetInstance()->GetMinionAndOffsetRead
                                                    (task->GetData().
                                                    m_from);

    // 3. Send request:
    UID uid = minion_and_offest.first->Read(
                                            minion_and_offest.second,
                                            task);

    // Debug:
    cout << "Send to minion: key = " << task->GetKey()
         << " key = " << task->GetData().m_key 
         << "from = " << task->GetData().m_from
         << " len = " << task->GetData().m_len
         << endl;
    uid.Print();

    size_t counter = 0;

    std::function<bool()> AsyncFunction = [uid, counter]() mutable
    {
        ResponsManager::p_data data_uid = Singleton<ResponsManager>::GetInstance()->GetResponseData(uid);
        if (data_uid.first == MinionUtility::SUCCESS || data_uid.first == MinionUtility::FAIL)
        {
            Singleton<ResponsManager>::GetInstance()->ClearResponseStatus(uid);

            return true; 
        }
        else // status == NO_RESPONSE_YET
        {
            if (counter < 10)
            {
                //std::cout << "async injection in if" << std::endl;

                ++counter;
                return false;			
            }
            //std::cout << "async injection after if!!!!" << std::endl;

            Singleton<ResponsManager>::GetInstance()->ClearResponseStatus(uid);

            return true;			
        }
    };

    return NBDCommandRead::p_async_response(AsyncFunction, std::chrono::milliseconds(1000));
}
