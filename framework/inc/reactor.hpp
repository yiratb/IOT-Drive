#ifndef ILRD_RD1556_REACTOR_HPP
#define ILRD_RD1556_REACTOR_HPP

#include <vector> //std::vector
#include <unordered_map> //std::unordered_map
#include <functional> //std::function
#include <sys/epoll.h>

class FdListener
{  
public:
    enum event_type
    {
        // Debug: fix
        WRITE = EPOLLOUT,//EPOLLIN,
        READ = EPOLLIN,//EPOLLOUT,
        ERROR = EPOLLERR
    };

    using p_key = std::pair<int, event_type>;

    explicit FdListener();
    ~FdListener();

    void Add(int fd_, event_type event_type_);
    void Remove(int fd_, event_type event_type_);
    std::vector<p_key> Wait();

private:
    int m_epoll_fd;
    std::unordered_map < int, uint32_t > m_listened_events;
};

class Reactor
{
public:

    explicit Reactor();
    ~Reactor();
    Reactor(const Reactor& other_) = delete;
    Reactor& operator=(const Reactor& other_) = delete;

    using event_type = FdListener::event_type;
    static const event_type READ = event_type::READ;
    static const event_type WRITE = event_type::WRITE;
    static const event_type ERROR = event_type::ERROR;

    void Add(int fd_, event_type event_type_, std::function<void()> func_);
    void Remove(int fd_, event_type event_type_);
    void Run(); //blocking
    void Stop();
    static std::string path;

private:
    using p_key = std::pair<int, event_type>;

    struct pair_hash 
    {
        std::size_t operator () (const p_key &p) const 
        {
            // Debug: 
            // - Might this hash function be inconsistent / no unique?
            return std::hash<int>()(p.first);
            //return std::hash<int>()(p.first) ^ std::hash<int>()(p.second);
        }
    };

    bool m_is_running;
    std::unordered_map < p_key, std::function <void()>, pair_hash > m_handlers;
    FdListener m_fd_listener;
};


#endif /* ILRD_RD1556_REACTOR_HPP */