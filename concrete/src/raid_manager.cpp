#include "raid_manager.hpp"
#include "logger.hpp"

RaidManager::RaidManager() : MINION_DISK_SPACE(4 * 1024 * 1024)
{
    m_minion_proxies.push_back(new MinionProxy("127.0.0.1", 4000));
    m_minion_proxies.push_back(new MinionProxy("127.0.0.1", 4500));
    m_minion_proxies.push_back(new MinionProxy("127.0.0.1", 6000));
}

RaidManager::~RaidManager()
{
    for (size_t i = 0; i < 3; i++)
    {
        delete m_minion_proxies[i];
        m_minion_proxies[i] = nullptr;
    }
}

RaidManager::p_minion_proxy_pair RaidManager::GetMinionAndOffsetWrite(uint64_t offset)
{
    Logger *logger = Singleton<Logger, const std::string&>::GetInstance("logger_cpf");

    logger->Log("RaidManager: Get Minion And Offset Write" ,Logger::INFO);

    RaidManager::p_minion_proxy_pair minion_and_offest;

    size_t minion_index1 = offset / MINION_DISK_SPACE;
    // first minion:
    minion_and_offest.first.first = m_minion_proxies[minion_index1];
    minion_and_offest.first.second = offset % MINION_DISK_SPACE;

    size_t minion_index2 = (minion_index1 + 1) % m_minion_proxies.size();
    // Second minion:
    minion_and_offest.second.first = m_minion_proxies[minion_index2];
    minion_and_offest.second.second = MINION_DISK_SPACE + offset % MINION_DISK_SPACE;

    return minion_and_offest;
}

RaidManager::p_minion_proxy RaidManager::GetMinionAndOffsetRead(uint64_t offset)
{
    Logger *logger = Singleton<Logger, const std::string&>::GetInstance("logger_cpf");
    logger->Log("RaidManager: Get Minion And Offset Read" ,Logger::INFO);
    
    RaidManager::p_minion_proxy minion_and_offest;


    size_t minion_index = offset / MINION_DISK_SPACE;

    minion_and_offest.first = m_minion_proxies[minion_index];
    minion_and_offest.second = offset % MINION_DISK_SPACE;


    return minion_and_offest;
}