#include "response_manager.hpp"
#include "reply_write.hpp"
#include "reply_read.hpp"
#include "ip.hpp"

ResponsManager::p_data ResponsManager::GetResponseData(UID uid_)
{
    return response_status[uid_];
}

void ResponsManager::ClearResponseStatus(UID uid_)
{
    response_status.erase(uid_);
}

void ResponsManager::SetStatus(UID uid_, MinionUtility::Status status)
{
    response_status[uid_].first = status;
}

void ResponsManager::Add(UID uid_, p_data data)
{
    response_status[uid_] = data;
}
