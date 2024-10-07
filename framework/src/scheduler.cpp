#include <cstring> //memset
#include <csignal>
#include <ctime> //struct sigevent

#include "scheduler.hpp"
#include <chrono> //std::chro::seconds
#include <iostream>

const int MILLISECONDS_IN_SECOND = 1000;
const int NANOSECONDS_IN_MILLISECOND = 1000000;

Scheduler::Scheduler()
{
    struct sigevent sev;

    memset(&sev, 0, sizeof(sev));
    //when the timer expires, a new thread will be created to run TimerHandler
    sev.sigev_notify = SIGEV_THREAD;
    // The sigev_value union allows passing data to the notification function
    sev.sigev_value.sival_ptr = this;
    //This specifies the function to call when the timer expires
    sev.sigev_notify_function = TimerHandler;
    sev.sigev_notify_attributes = nullptr;

    if (-1 == timer_create(CLOCK_REALTIME, &sev, &m_timer_id)) 
    {
        throw std::runtime_error("Fail in timer_create");
    }
}

Scheduler::~Scheduler()
{
    timer_delete(m_timer_id);
}

void Scheduler::TimerHandler(sigval sv)
{   
    Scheduler *sched = static_cast<Scheduler*>(sv.sival_ptr);
    p_key task;

    {
        std::lock_guard<std::mutex> lock(sched->m_mutex_);
        task = sched->m_tasks.top();
    }

    if (task.first <=  sched->ConvertToDuration(std::chrono::system_clock::now()))
    {
        {
            std::lock_guard<std::mutex> lock(sched->m_mutex_);
            sched->m_tasks.pop();
            if (!sched->m_tasks.empty())
            {
                sched->UpdateTime();
            }
        }
        task.second->Execute();
    }
}

void Scheduler::Add(std::shared_ptr<ITask> task_, std::chrono::milliseconds time_to_exec_)
{
    auto exec_time_chrono = time_now() + time_to_exec_;
    // Convert std::chrono::time_point to time_t
    time_t exec_time = ConvertToDuration(exec_time_chrono);

    p_key key(exec_time, task_);

    std::lock_guard<std::mutex> lock(m_mutex_);

    m_tasks.push(key);

    UpdateTime();
}

void Scheduler::UpdateTime()
{
    struct itimerspec its;

    auto time_now_chrono = time_now();
    time_t time_now = ConvertToDuration(time_now_chrono);

    time_t new_time = m_tasks.top().first;

    auto diff = new_time - time_now;

    its.it_value.tv_sec = diff > 0 ? diff / MILLISECONDS_IN_SECOND : 0;
    its.it_value.tv_nsec = diff > 0 ? (diff % MILLISECONDS_IN_SECOND)
                                             * NANOSECONDS_IN_MILLISECOND : 1;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;

    // Set the timer with the calculated expiration time
    if (-1 == timer_settime(m_timer_id, 0, &its, nullptr)) 
    {
        throw std::runtime_error("Fail in timer_settime");
    }
}

time_t Scheduler::ConvertToDuration(const std::chrono::time_point<std::chrono::system_clock> time_point) 
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch()).count();
}

std::chrono::time_point<std::chrono::system_clock> Scheduler::time_now() 
{
    return std::chrono::system_clock::now();
}