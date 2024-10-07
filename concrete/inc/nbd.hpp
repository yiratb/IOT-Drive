#ifndef ILRD_RD1556_NBD_HPP
#define ILRD_RD1556_NBD_HPP

#include <thread> //thread

class NBD
{
public:
    explicit NBD(size_t size, std::string path);
    NBD(const NBD& other_) = delete;
    NBD& operator=(const NBD& other_) = delete;
    ~NBD();
    int GetFd();
    int WriteAll(int fd, char* buf, size_t count);
    void ListenNbd();

private:
    static void DisconnectNbd(int signal);
    void ThreadNBD();

    static int nbd_dev_to_disconnect;

    int m_fd_nbd;
    int m_socket_pair[2];
    std::thread m_nbd_thread;
};

#endif //ILRD_RD1556_NBD_HPP