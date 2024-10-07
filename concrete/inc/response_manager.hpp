#ifndef ILRD_RD1556_RESPONSE_MANAGER_HPP
#define ILRD_RD1556_RESPONSE_MANAGER_HPP

#include <unordered_map>
#include <memory>
#include <nbd_key_task.hpp>

#define ALLOWED_SINGLETON
#include "singleton.hpp" //Singleton
#undef ALLOWED_SINGLETON
#include "uid.hpp"
#include "minion_utility.hpp"

class ResponsManager
{
public:
    ~ResponsManager() = default;
    ResponsManager(const ResponsManager& other_) = delete;
    ResponsManager& operator=(const ResponsManager& other_) = delete;
    void CallBack();

    using p_data = std::pair< MinionUtility::Status, std::shared_ptr<NBDKeyTask> >;

    p_data GetResponseData(UID uid_);
    void ClearResponseStatus(UID uid_);
    void SetStatus(UID uid_, MinionUtility::Status status);
    void Add(UID uid_, p_data data);

private:
    explicit ResponsManager() = default;
    friend Singleton<ResponsManager>;

    std::unordered_map<UID, p_data, UID::HashFunction> response_status;

    std::string m_ip;
    size_t m_port;
    int m_listen_socket_fd;
};

#endif //ILRD_RD1556_RESPONSE_MANAGER_HPP