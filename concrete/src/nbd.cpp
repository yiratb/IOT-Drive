#include <cstring> //memset
#include <sys/socket.h> //socketpair
#include <unistd.h> //write
#include <fcntl.h> //open
#include <sys/ioctl.h> //ioctl
#include <sys/wait.h> //sigfillset, sigprocmask, SIG_SETMASK
#include <linux/nbd.h> //NBD_DO_IT, NBD_SET_SOCK...
#include <functional> //bind
#include <iostream>

#include "logger.hpp"
#include "nbd.hpp"


int NBD::nbd_dev_to_disconnect = -1;

NBD::NBD(size_t size, std::string path)
{
    std::cout << "NBD() path: " << path << "\n";

    Logger *logger = Singleton<Logger, const std::string&>::GetInstance("logger_cpf");

    logger->Log("NBD start ctor", Logger::INFO);

    int err = socketpair(AF_UNIX, SOCK_STREAM, 0, m_socket_pair);
    if (-1 == err)
    {
        throw std::runtime_error("socketpair failed in NBDManager Ctor");
    }
    
    m_fd_nbd = open(path.c_str(), O_RDWR);
    if (m_fd_nbd == -1) 
    {
        throw std::runtime_error("open(m_nbd_path, O_RDWR) failed in "
                                 "NBDManager Ctor");
    }

    err = ioctl(m_fd_nbd, NBD_SET_SIZE, size);
    if (-1 == err)
    {
        throw std::runtime_error("ioctl(m_fd_nbd, NBD_SET_SIZE, size_) "
                                 "failed in NBDManager Ctor");
    }

    err = ioctl(m_fd_nbd, NBD_CLEAR_SOCK);
    if (-1 == err)
    {
        throw std::runtime_error("ioctl(m_fd_nbd, NBD_CLEAR_SOCK) failed in "
                                 "NBDManager Ctor");
    }

    // Debug:
    std::cout << "NBD::NBD() done.\n";
}

NBD::~NBD()
{
    /* Logger *logger = Singleton<Logger, const std::string&>::GetInstance("logger_cpf");

    logger->Log("NBD start dtor" ,Logger::INFO);
    //DisconnectNbd();
    if (m_nbd_thread.joinable())
    {
        m_nbd_thread.join();
    } */
}

void NBD::ListenNbd()
{
    m_nbd_thread = std::thread(std::bind(&NBD::ThreadNBD, this));
}

int NBD::GetFd()
{
    return m_socket_pair[0];
}

int NBD::WriteAll(int fd, char* buf, size_t count)
{
    Logger *logger = Singleton<Logger, const std::string&>::GetInstance("logger_cpf");

    int bytes_written;

    logger->Log("NBDProxy Before Reply Fail", Logger::INFO);
    while (count > 0) 
    {

        bytes_written = write(fd, buf, count); // Write from buffer to file descriptor

        if (bytes_written <= 0) //Ensure that bytes were written
        {
            throw std::runtime_error("WriteAll fail in the while\n" + std::string(strerror(errno)));
        }

        buf += bytes_written; // Move buffer pointer forward
        count -= bytes_written; // Decrease count by the number of bytes written
    }
    logger->Log("NBDProxy After Reply Fail", Logger::INFO);
    if (0 != count) // Ensure all bytes were written
    {
        throw std::runtime_error("WriteAll fail\n");
    }

    return 0;
}

void NBD::DisconnectNbd(int signal)
{
    (void)signal;
    if (nbd_dev_to_disconnect != -1) 
    {
        if(ioctl(nbd_dev_to_disconnect, NBD_DISCONNECT) == -1) 
        {
            throw std::runtime_error("failed to request disconect on nbd device");
        } 
        else 
        {
            nbd_dev_to_disconnect = -1;
            std::cout << "sucessfuly requested disconnect on nbd device\n" <<std::endl;
        }
    }
}

void NBD::ThreadNBD()
{
    // Debug:
    std::cout << "ThreadNBD() 1\n";

    Logger *logger = Singleton<Logger, const std::string&>::GetInstance("logger_cpf");

    // Debug:
    std::cout << "ThreadNBD() 2\n";


    sigset_t sigset;
    if (sigfillset(&sigset) != 0 ||
        sigprocmask(SIG_SETMASK, &sigset, NULL) != 0) 
    {
        throw std::runtime_error("sigfillset(&sigset) or "
        "sigprocmask(SIG_SETMASK, &sigset, NULL) "
        "failed in NBDManager StartListen");
    }

    /* The child needs to continue setting things up. */
    int sk = m_socket_pair[1];

    // Debug:
    std::cout << "ThreadNBD() 3\n";
    std::cout << "m_fd_nbd: " << m_fd_nbd << "m_socket_pair [0][1]: " << m_socket_pair[0] << "," << m_socket_pair[1] << "\n";

    if(ioctl(m_fd_nbd, NBD_SET_SOCK, sk) == -1)
    {
        // Debug:
        std::cout << "ThreadNBD() 3.5\n";

        throw std::runtime_error("ioctl(m_fd_nbd, NBD_SET_SOCK, sk) "
                                    "failed in NBDManager StartListen");
    }
    else
    {
        // Debug:
        std::cout << "ThreadNBD() 3.6\n";

        #if defined NBD_SET_FLAGS
            int flags = 0;
            #if defined NBD_FLAG_SEND_TRIM
                flags |= NBD_FLAG_SEND_TRIM;
            #endif
            #if defined NBD_FLAG_SEND_FLUSH
            flags |= NBD_FLAG_SEND_FLUSH;
            #endif

            if (flags != 0 && ioctl(m_fd_nbd, NBD_SET_FLAGS, flags) == -1)
            {
                throw std::runtime_error("NBD flag setup failed in "
                                            "NBDManager StartListen");
            }


        #endif

        logger->Log("Befor ioctl" ,Logger::INFO);

        // Debug:
        std::cout << "ThreadNBD() 4.5 - before NBD_DO_IT\n";

        int err = ioctl(m_fd_nbd, NBD_DO_IT);
        if (err == -1) 
        {
            throw std::runtime_error("ioctl(m_fd_nbd, NBD_DO_IT) failed");
        }

        logger->Log("After ioctl" ,Logger::INFO);

    }

    // Debug:
    std::cout << "ThreadNBD() 4\n";

    if (ioctl(m_fd_nbd, NBD_CLEAR_QUE) == -1 ||
                ioctl(m_fd_nbd, NBD_CLEAR_SOCK) == -1) 
    {
        throw std::runtime_error("m_fd_nbd, NBD_CLEAR_QUE) or  "
        "ioctl(m_fd_nbd, NBD_CLEAR_SOCK) failed in NBDManager StartListen");
    }

    // Debug:
    std::cout << "ThreadNBD() 5\n";
}
