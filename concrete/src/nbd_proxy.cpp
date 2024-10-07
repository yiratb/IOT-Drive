#include <unistd.h> //close
#include <netinet/in.h> //ntohl
#include <cstring> //memcpy

/* #include <iostream>
#include <cerrno> */
/* #include <sys/socket.h> */
/* #include <cstdlib> */

#include <linux/nbd.h>
#include <fcntl.h>

#include "nbd_proxy.hpp"
#include "nbd_key_task.hpp"
#include "logger.hpp"

#include <iostream>
using namespace std;
/*
 * Converts 64-bit integers between host and network byte order, depending on the system's endianness.
 */
#ifdef WORDS_BIGENDIAN
u_int64_t ntohll(u_int64_t a) 
{
  return a;
}
#else
u_int64_t ntohll(u_int64_t a) 
{
  u_int32_t lo = a & 0xffffffff;
  u_int32_t hi = a >> 32U;
  lo = ntohl(lo);
  hi = ntohl(hi);
  return ((u_int64_t) lo) << 32U | hi;
}
#endif
#define htonll ntohll

// Debug:
void NbdProxy::PrintReply(const nbd_reply& reply)
{
    cout << "nbd_reply: "
        << " magic = " << reply.magic
	    << " error (0=ok) = " << reply.error		/* 0 = ok, else error	*/
	    << " chars = ";		/* handle you got from request	*/
    for (int i = 0 ; i < 8 ; ++i)
    {
        cout << reply.handle[i];
        if (i < 7)
            cout << ",";
        else
            cout << endl;
    }
}

NbdProxy::~NbdProxy()
{
    //close(m_fd);
}

#include <chrono>
#include <thread>

std::shared_ptr<IKeyTask> NbdProxy::GetTask()
{
    //std::lock_guard<std::mutex> lock(m_mutex);

    ssize_t bytes_read;
    nbd_request request;
    nbd_reply reply;
    uint32_t len;
    uint64_t from;
    char *chunk = nullptr;


    Logger *logger = Singleton<Logger, const std::string&>::GetInstance("logger_cpf");
    logger->Log("NBDProxy GetTask", Logger::INFO);
    std::cout << "NBDProxy GetTask" << std::endl;

    logger->Log("NBDProxy Before Fail", Logger::INFO);

    bytes_read = read(m_fd, &request, sizeof(request));
    if (bytes_read != sizeof(request)) 
    {
        throw std::runtime_error("error reading userside of nbd socket");
    }
    logger->Log("NBDProxy After Fail", Logger::INFO);

    memcpy(reply.handle, request.handle, sizeof(reply.handle));
    reply.magic = htonl(NBD_REPLY_MAGIC);
    reply.error = htonl(0);

    len = ntohl(request.len);
    from = ntohll(request.from);
    auto type = ntohl(request.type);

    std::cout << "From " << from << std::endl;
    std::cout << "Len1 " << len << std::endl;

    chunk = new char[len];
    //memset(chunk, 0, len);

    if (type == NBD_CMD_WRITE)
    {
        read_all(m_fd, chunk, len); // Read data from socket
    }

    // Debug:
    // std::this_thread::sleep_for(200ms);

    logger->Log("NBDProxy Finish GetTask", Logger::INFO);

    // Debug:
    cout << "NBD requested (*): type = " << type << " from = " << from << " len = " << len << endl;
    PrintReply(reply);

    // Debug:
    // - This might be the fix:
    std::shared_ptr<NBDKeyTask> task = std::make_shared<NBDKeyTask>(type, len, from, chunk, reply);    
    if (type != NBD_CMD_WRITE && type != NBD_CMD_READ)
    {
        // Reply immediately:
        this->SendResponse(task,nullptr);
        return nullptr;
    }

    return task;
}

void NbdProxy::SetFd(int fd_)
{
    m_fd = fd_;
}

void NbdProxy::SetNBD(NBD *nbd_)
{
    m_nbd = nbd_;
}

void NbdProxy::SendResponse(std::shared_ptr<NBDKeyTask> nbd_key_task_, char *buffer_)
{
    // std::cout << "Reply " << (char *)&(nbd_key_task_->GetData().m_reply) << std::endl;

    // Debug:
    cout << "Replying to NBD: ";
    cout << " key = " << nbd_key_task_->GetData().m_key 
         << " from = " << nbd_key_task_->GetData().m_from
         << " len = " << nbd_key_task_->GetData().m_len << endl;
    PrintReply(nbd_key_task_->GetData().m_reply);


    m_nbd->WriteAll(m_fd, (char *)&(nbd_key_task_->GetData().m_reply), sizeof(nbd_reply));
    if (nullptr != buffer_)
    {
        std::cout << "Chunk " << buffer_ << std::endl;
        std::cout << "Len2 " << nbd_key_task_->GetData().m_len << std::endl;
        m_nbd->WriteAll(m_fd, buffer_, nbd_key_task_->GetData().m_len);
    }
}

NbdProxy::NbdProxy()
{
    std::cout << "-----------------------------------------------\n";
    sem_init(&m_semaphore, 1, 2);
}

int NbdProxy::read_all(int fd, char *buff, size_t len)
{
    int bytes_read;

    while (len > 0) 
    {
        bytes_read = read(fd, buff, len); // Read from file descriptor into buffer
        if (bytes_read <= 0) //Ensure that bytes were read
        {
            throw std::runtime_error("NbdProxy::read_all: error reading userside of nbd socket");
        }
        buff += bytes_read; // Move buffer pointer forward
        len -= bytes_read; // Decrease len by the number of bytes read
    }
    if (len != 0) //Ensure that bytes were read
    {
        throw std::runtime_error("NbdProxy::read_all: len != 0");
    }

    return 0;
}