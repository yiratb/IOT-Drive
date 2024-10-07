#include "async_injection.hpp"

#include <iostream>

AsyncInjection::AsyncInjection(std::function<bool()> action_, 
                            std::chrono::milliseconds timeout_) :
                                m_action(action_), m_timeout(timeout_)
{
    m_sched = Singleton<Scheduler>::GetInstance();

    m_task = std::make_shared<AsyncInjectionTask>(this);
    m_sched->Add(m_task, m_timeout);
}

AsyncInjection::AsyncInjectionTask::AsyncInjectionTask
        (AsyncInjection *async_injection) 
        : m_async_injection(async_injection)
{

}

void AsyncInjection::AsyncInjectionTask::Execute()
{
    if (true == m_async_injection->m_action())
    {
        delete m_async_injection;
        m_async_injection = nullptr;
    }
    else
    {
        m_async_injection->m_sched->
                    Add(m_async_injection->m_task, m_async_injection->m_timeout);
    }
}
