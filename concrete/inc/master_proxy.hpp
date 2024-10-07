#ifndef ILRD_RD1556_MASTER_PROXY_HPP
#define ILRD_RD1556_MASTER_PROXY_HPP

#include <netinet/in.h> //sockaddr_in, SOCK_DGRAM, connect

#define ALLOWED_SINGLETON
#include "singleton.hpp"
#undef ALLOWED_SINGLETON
#include "iinput_proxy.hpp"
#include "minion_utility.hpp"

class MasterProxy : public IInputProxy
{
public:
    ~MasterProxy() = default;
    MasterProxy(const MasterProxy& other_) = delete;
    MasterProxy& operator=(const MasterProxy& other_) = delete;
    std::shared_ptr<IKeyTask> GetTask();
    void SendResponseWrite(MinionUtility::Status status, std::shared_ptr<IKeyTask> task_);
    void SendResponseRead(MinionUtility::Status status, std::shared_ptr<IKeyTask> task_);
    void SetFd(int socket_fd_);
private:
    explicit MasterProxy();
    friend Singleton<MasterProxy>;
    
    std::string m_ip;
    size_t m_port;
    struct sockaddr_in m_servaddr;
    int m_listen_socket_fd;
    int m_send_socket_fd;
};

#endif //ILRD_RD1556_MASTER_PROXY_HPP