#ifndef __ILRD__WQUEUE__
#define __ILRD__WQUEUE__

#include <chrono>               // chrono::milliseconds
#include <queue>                // std::queue
#include <mutex>                // std::timed_mutex
#include <condition_variable>   // std::condition_variable

template <typename T, typename CONTAINER = std::queue<T> >
class WQueue
{
public:
    WQueue() = default;
    ~WQueue() = default;
    WQueue(const WQueue &other_) = delete;
    WQueue& operator=(const WQueue& other_) = delete;

    bool Pop(T& out_param_, std::chrono::milliseconds timeout);
    void Pop(T& out_param_);
    void Push(const T& element_);
    bool IsEmpty() const;

private:
    CONTAINER m_queue;
    mutable std::timed_mutex m_lock;
    std::condition_variable_any m_cond;
};

template <typename T, typename CONTAINER>
bool WQueue<T, CONTAINER>::Pop(T& out_param_, std::chrono::milliseconds timeout)
{
    std::unique_lock<std::timed_mutex> lock(m_lock);

    if (!m_cond.wait_for(lock, timeout, [this] { return !m_queue.empty(); }))
    {
        return false;
    }

    out_param_ = m_queue.front();
    m_queue.pop();

    return true;
}

template <typename T, typename CONTAINER>
void WQueue<T, CONTAINER>::Pop(T& out_param_)
{
    std::unique_lock<std::timed_mutex> lock(m_lock);
    
    m_cond.wait(lock, [this] { return !m_queue.empty(); });
    out_param_ = m_queue.front();
    m_queue.pop();
}

template <typename T, typename CONTAINER>
void WQueue<T, CONTAINER>::Push(const T& element_) 
{
    std::lock_guard<std::timed_mutex> lock(m_lock);
    m_queue.push(element_);

    m_cond.notify_one();
}

template <typename T, typename CONTAINER>
bool WQueue<T, CONTAINER>::IsEmpty() const
{
    std::lock_guard<std::timed_mutex> lock(m_lock);
    return m_queue.empty();
}

#endif /*__ILRD__WQUEUE__*/