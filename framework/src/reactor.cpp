#include <iostream>
#include <unistd.h> //close
#include <string.h>

#include "reactor.hpp"
#include "logger.hpp" //logger

std::string Reactor::path("logger_cpf");

Reactor::Reactor() : m_is_running(false)
{

}

Reactor::~Reactor()
{
    m_handlers.clear();
}

void Reactor::Add(int fd_, event_type event_type_, std::function<void()> func_)
{
    m_fd_listener.Add(fd_, event_type_);

    p_key key = {fd_, event_type_};
    m_handlers[key] = func_;
}

void Reactor::Remove(int fd_, event_type event_type_)
{
    m_fd_listener.Remove(fd_, event_type_);

    p_key key = {fd_, event_type_};
    m_handlers.erase(key);
}

void Reactor::Run()
{
    Logger *logger = Singleton<Logger, const std::string&>::GetInstance(Reactor::path);
     
    m_is_running = true;

    logger->Log("Reactor: Start Run" ,Logger::INFO); 

    while (m_is_running)
    {
        // Debug:
        // std::cout << "Before m_fd_listener.Wait()\n";

        for (auto event: m_fd_listener.Wait())
        {
            // Debug:
            //std::cout << "After m_fd_listener.Wait()\n";

            logger->Log("Reactor: event occurred" ,Logger::INFO); 
            m_handlers[event]();
        }
    }
}

void Reactor::Stop()
{
    m_is_running = false;
}

FdListener::FdListener()
{
    m_epoll_fd = epoll_create1(0);
    if (-1 == m_epoll_fd) 
    {
        throw std::runtime_error("fail in epoll_create1");
    }
}

FdListener::~FdListener()
{
    close(m_epoll_fd);
}

void FdListener::Add(int fd_, event_type event_type_)
{
    Logger *logger = Singleton<Logger, const std::string&>::GetInstance(Reactor::path);
    struct epoll_event event;
    int flag = 0;
    memset(&event, 0, sizeof(event));

    event.data.fd = fd_;

    if (m_listened_events.find(fd_) == m_listened_events.end())
    {
        m_listened_events[fd_] = event_type_;
        flag = EPOLL_CTL_ADD;
        logger->Log("Reactor: Add new fd" ,Logger::INFO); 
    }
    else
    {
        m_listened_events[fd_] |= event_type_;
        flag = EPOLL_CTL_MOD;
        logger->Log("Reactor: Add - mode fd" ,Logger::INFO); 
    }
    event.events = m_listened_events[fd_];

    if (-1 == epoll_ctl(m_epoll_fd, flag, fd_, &event)) 
    {
        std::string error_message = "fail in epoll_ctl with flag " + std::to_string(flag) + ": " + strerror(errno);
        throw std::runtime_error(error_message);

    }
}

void FdListener::Remove(int fd_, event_type event_type_)
{
    Logger *logger = Singleton<Logger, const std::string&>::GetInstance(Reactor::path);
    struct epoll_event event;
    int flag = EPOLL_CTL_MOD;

    memset(&event, 0, sizeof(event));
    event.data.fd = fd_;

    if (m_listened_events.find(fd_) != m_listened_events.end())
    {
        m_listened_events[fd_] = m_listened_events[fd_] & (~event_type_);
        event.events = m_listened_events[fd_];

        if (0 == m_listened_events[fd_])
        {
            flag = EPOLL_CTL_DEL;
            m_listened_events.erase(fd_);
            logger->Log("Reactor: Delete fd" ,Logger::INFO); 
        }
        else
        {
            logger->Log("Reactor: Delete - mode fd" ,Logger::INFO); 
        }
        
        if (-1 == epoll_ctl(m_epoll_fd, flag, fd_, &event)) 
        {
            throw std::runtime_error("fail in epoll_ctl remove");
        }
    }
}

std::vector<FdListener::p_key> FdListener::Wait()
{
    std::vector<p_key> activate;
    const int MAX_EVENTS_AT_A_TIME = 10;
    struct epoll_event events[MAX_EVENTS_AT_A_TIME];
    const int TIME_OUT = 200;
    Logger *logger = Singleton<Logger, const std::string&>::GetInstance(Reactor::path);

    int count_fds = epoll_wait(m_epoll_fd, events, MAX_EVENTS_AT_A_TIME, TIME_OUT);
    if (-1 == count_fds)
    {
        throw std::runtime_error("fail in epoll_wait");
    }

    for (int i = 0; i < count_fds; ++i)
    {
        logger->Log("Reactor: Wait" ,Logger::INFO); 
        int fd = events[i].data.fd;

        // Debug:
        // - Switch. IN == READ...
        if (events[i].events &  EPOLLIN)
        {
            activate.push_back(p_key(fd, READ));//WRITE));
        }

        if (events[i].events &  EPOLLOUT)
        {
            activate.push_back(p_key(fd, WRITE));//READ));
        }

        if (events[i].events &  EPOLLERR)
        {
            activate.push_back(p_key(fd, ERROR));
        }
        
    }

    return activate;
}