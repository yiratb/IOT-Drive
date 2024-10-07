/****************************************************                                                        
 *  Written by: Yirat Romano                         
 *                                                  
 *  Reviewed by: Alon                        
 *                                                  
 *  Completed on - 01/07/24                        
 *                                                  
 ****************************************************/
#include <sys/inotify.h> //inotify_init
#include <iostream> // cout
#include <unistd.h> //read
#include <dlfcn.h> //dlopen, dlclose
#include <functional> // std::function

#include "factory.hpp" //factory
#include "dir_monitor.hpp" //dir_momitor

DirMonitor::DirMonitor(const std::string pathname_) : m_pathname(pathname_), m_is_running(false)
{
    m_fd = inotify_init();
    if (m_fd < 0)
    {
        throw std::runtime_error("inotify_init fail\n");
    }

    int wd = inotify_add_watch(m_fd, pathname_.c_str(),IN_MODIFY | IN_CREATE | IN_MOVED_TO | IN_DELETE);
    if (wd < 0)
    {
        close(m_fd);
        throw std::runtime_error("inotify_add_watch fail\n");
    }
}

DirMonitor::~DirMonitor()
{
    m_is_running = false;
    if (m_thread.joinable())
	{
		m_thread.join();
	}

    close(m_fd);
}

void DirMonitor::StartMonitor()
{
    m_is_running = true;
    m_thread = std::thread(std::bind(&DirMonitor::Monitor, this));
}

Dispatcher<std::string> &DirMonitor::GetDispatcher()
{
    return m_dispatcher;
}

void DirMonitor::Monitor()
{
    const int EVENT_SIZE = sizeof (struct inotify_event);
    const int BUF_LEN = 1024 * (EVENT_SIZE + 16);
    char buffer[BUF_LEN];
    fd_set read_fds;

    while (m_is_running)
    {
        FD_ZERO(&read_fds);
        FD_SET(m_fd, &read_fds);
        struct timeval timeout = {1, 0};
        struct inotify_event *event;

        int activity = select(m_fd + 1, &read_fds, NULL, NULL, &timeout);
        if (activity < 0) 
        {
            throw std::runtime_error("select fail\n");
        }

        if (activity > 0 && FD_ISSET(m_fd, &read_fds)) 
        {
            std::cout << "i am in" << std::endl;
            if (read(m_fd, buffer, BUF_LEN) < 0)
            {
                throw std::runtime_error("Failed to read inotify watch\n");
            }

            event = (struct inotify_event *)buffer;
            if (IN_CREATE == event->mask || IN_MOVED_TO == event->mask)
            {
                m_dispatcher.Broadcast(m_pathname + "/" + event->name);
            }
        }
    }    
}

DllLoader::~DllLoader()
{
    for (auto lib : m_fds)
    {
        dlclose(lib);
        lib = nullptr;
    }
    m_fds.clear();
}

void DllLoader::Notify(std::string data_)
{
    void *lib = dlopen(data_.c_str(), RTLD_LAZY);

    if (lib == nullptr) 
    {
        throw std::runtime_error("dlopen fail\n");
    }
    m_fds.push_back(lib);
}

void DllLoader::DeathExec()
{
}