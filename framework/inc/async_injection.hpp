
#ifndef __RD1556_ASYNC_INJECTION_HPP__
#define __RD1556_ASYNC_INJECTION_HPP__

#include <functional> //std::function
#include <chrono> //std::chrono::milliseconds

#include "scheduler.hpp"

class AsyncInjection
{
public:
    AsyncInjection(std::function<bool()> action_, std::chrono::milliseconds timeout_);
    AsyncInjection(const AsyncInjection& other_) = delete;
    AsyncInjection& operator=(const AsyncInjection &other_) = delete;

private:
    ~AsyncInjection() = default;

    class AsyncInjectionTask : public Scheduler::ITask
    {
    public:   
        AsyncInjectionTask(AsyncInjection *parent);
        ~AsyncInjectionTask() = default;
        void Execute();

    private:
        AsyncInjection *m_async_injection;    
    };
    
    Scheduler *m_sched;
    std::function<bool()> m_action;
    std::shared_ptr<AsyncInjectionTask> m_task;
    std::chrono::milliseconds m_timeout;
};

#endif //__RD1556_ASYNC_INJECTION_HPP__