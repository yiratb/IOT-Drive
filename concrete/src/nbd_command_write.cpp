#include "nbd_command_write.hpp"
#include "nbd_key_task.hpp"
#include "uid.hpp"
#include "raid_manager.hpp"
#include "response_manager.hpp"
#include "logger.hpp"
#include "minion_utility.hpp"

NBDCommandWrite::p_async_response NBDCommandWrite::Run(std::shared_ptr<IKeyTask> task_)
{
    Logger *logger = Singleton<Logger, const std::string&>::GetInstance("logger_cpf");
    logger->Log("NBD Command Write Run" ,Logger::INFO);
    // 1. Get args:
    std::shared_ptr<NBDKeyTask> task = std::dynamic_pointer_cast<NBDKeyTask>(task_);

    // 2. Get Minion Proxy and offset:
    RaidManager::p_minion_proxy_pair minion_and_offest = 
            Singleton<RaidManager>::GetInstance()->GetMinionAndOffsetWrite
                                                    (task->GetData().
                                                    m_from);

    // 3. Send request:
    UID uid1 = minion_and_offest.first.first->Write(
                                            minion_and_offest.first.second,
                                            task);

    // Debug: 
    // One minion test, send to just one (although should work, just sending to UDP socket...)
    /*UID uid2 = minion_and_offest.second.first->Write(
                                            minion_and_offest.second.second,
                                            task);*/

    // Debug:
    return NBDCommandWrite::p_async_response(nullptr, std::chrono::milliseconds(1000));

/*
    size_t counter = 0;

    std::function<bool()> AsyncFunction = [uid1, uid2, counter]() mutable
    {
        ResponsManager::p_data data_uid1 = Singleton<ResponsManager>::GetInstance()->GetResponseData(uid1);
        ResponsManager::p_data data_uid2 = Singleton<ResponsManager>::GetInstance()->GetResponseData(uid2);

        if ((data_uid1.first == MinionUtility::SUCCESS || data_uid1.first == MinionUtility::FAIL)
        && (data_uid2.first == MinionUtility::SUCCESS || data_uid2.first == MinionUtility::FAIL))
        {
            std::cout << "async injection success" << std::endl;

            Singleton<ResponsManager>::GetInstance()->ClearResponseStatus(uid1);
            Singleton<ResponsManager>::GetInstance()->ClearResponseStatus(uid2);

            return true; 
        }
        else //status == NO_RESPONSE_YET
        {   
            if (counter < 10)
            {
                ++counter;
                return false;			
            }

            Singleton<ResponsManager>::GetInstance()->ClearResponseStatus(uid1);
            Singleton<ResponsManager>::GetInstance()->ClearResponseStatus(uid2);

            return true;
        }
    };

    return NBDCommandWrite::p_async_response(AsyncFunction, std::chrono::milliseconds(1000));
*/
}