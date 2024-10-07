#ifndef ILRD_RD1556_MINION_PROXY_HPP
#define ILRD_RD1556_MINION_PROXY_HPP

#include <netinet/in.h> //sockaddr_in, SOCK_DGRAM, connect
#include <memory> //std::shared_ptr
#include <mutex> //mutex

#include "nbd_key_task.hpp"
#include "uid.hpp"

class MinionProxy
{
public:
    explicit MinionProxy(std::string ip, size_t port);
    ~MinionProxy();
    UID Read(uint64_t offset, std::shared_ptr<NBDKeyTask> task);
    UID Write(uint64_t offset, std::shared_ptr<NBDKeyTask> task);
    void SetOffset();
    
private:
    std::mutex m_mutex;
    std::string m_ip;
    size_t m_port;
    int m_sockfd;
	struct sockaddr_in m_servaddr;
};

#endif //ILRD_RD1556_MINION_PROXY_HPP