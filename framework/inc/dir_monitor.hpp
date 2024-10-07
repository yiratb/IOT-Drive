/****************************************************                                                        
 *  Written by: Yirat Romano                         
 *                                                  
 *  Reviewed by: Alon                        
 *                                                  
 *  Completed on - 01/07/24                        
 *                                                  
 ****************************************************/
#ifndef ILRD_RD1556_DIR_MONITOR_HPP
#define ILRD_RD1556_DIR_MONITOR_HPP

#include <string> // std::string
#include <thread> // std::thread

#include "dispatcher.hpp" // dispatcher

/*
- Provide a path to the directory monitor to track.
- The monitor stops only when its destructor is called.
- After creating the monitor, you should add the desired dll loader inheriting
    from the observer class of dispatcher, or use the default one "DllLoader" 
    which uses the lib function dlopen and dlclose
- The constructor may throw simple string errors for invalid directory paths.
*/
class DirMonitor
{
public:
    explicit DirMonitor(const std::string pathname_);

    ~DirMonitor();
    DirMonitor(const DirMonitor& other_) = delete;
    DirMonitor& operator=(const DirMonitor &other_) = delete;
    void StartMonitor();
    Dispatcher<std::string> &GetDispatcher();

private:
    void Monitor();

    Dispatcher<std::string> m_dispatcher;
    std::string m_pathname;
    int m_fd;
    int m_is_running;
    std::thread m_thread;
};

/*
- Define the attribute constructor.
- Create the factory using the singleton pattern.
- Add methods to the factory.
- Ensure the attribute destructor removes the methods from the factory.
*/
class DllLoader: public IObserver<std::string>
{
public:
    DllLoader() = default;
    ~DllLoader();
    void Notify(std::string data_);
    void DeathExec();
    

private:
    std::vector<void *> m_fds;
};

#endif // ILRD_RD1556_DIR_MONITOR_HPP
