#ifndef ILRD_RD1556_NBD_PROXY_HPP
#define ILRD_RD1556_NBD_PROXY_HPP

#include <memory> //std::shared_ptr
#include <semaphore.h> // semaphore

#include "iinput_proxy.hpp"
#include "ikey_task.hpp"
#include "nbd.hpp"
#include "nbd_key_task.hpp"
#include "singleton.hpp"

class NbdProxy : public IInputProxy
{
public:
    ~NbdProxy();
    NbdProxy(const NbdProxy& other_) = delete;
    NbdProxy& operator=(const NbdProxy& other_) = delete;
    std::shared_ptr<IKeyTask> GetTask();
    void SetFd(int fd_);
    void SetNBD(NBD *nbd_);
    void SendResponse(std::shared_ptr<NBDKeyTask> nbd_key_task, char *buffer_);

    // Debug:
    void PrintReply(const nbd_reply& reply);

private:
    explicit NbdProxy(); 
    friend Singleton<NbdProxy>;
    int read_all(int fd, char *buf, size_t len);
    int m_fd;
    NBD *m_nbd;
    sem_t m_semaphore;
};

#endif //ILRD_RD1556_NBD_PROXY_HPP