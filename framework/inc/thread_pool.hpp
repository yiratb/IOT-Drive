/****************************************************                                                                                              
 *  Written by: Yirat Romano                          
 *                                                  
 *  Reviewed by:                         
 *                                                  
 *  Completed on - 20/06/24                         
 *                                                  
 ****************************************************/

/*
- Initialize the ThreadPool by creating an instance through the Singleton pattern.

- Derive your tasks from the Task class and implement the Run method.

- Add tasks to the ThreadPool using the Add method, specifying their priority.

- Refer to dispatcher.hpp for IObserver derivation details if needed.
*/

#ifndef __RD1556_THREADPOOL_HPP__
#define __RD1556_THREADPOOL_HPP__

#include <vector> /*std::vector*/
#include <unordered_map> /*std::unordered_map*/
#include <mutex> /*std::mutex*/
#include <future> /*std::future*/
#include <functional> /*std::function*/
#include <condition_variable> /*std::condition_variable*/

#include "wqueue.hpp" /*WQueue*/
#include "pqueue.hpp" /*PriorityQueue*/

#define ALLOWED_SINGLETON
#include "singleton.hpp" //Singleton
#undef ALLOWED_SINGLETON

class ThreadPool
{
public:
    ~ThreadPool();
    ThreadPool(const ThreadPool& other_) = delete;
    ThreadPool& operator=(const ThreadPool& other_) = delete;

    class Task
    {
    public:
        virtual void Run() = 0;
        virtual ~Task() = default;
    };

    template <typename T>
    class FutureTask : public Task
    {
    public:
        FutureTask(std::function<T()> func_);
        virtual ~FutureTask() = default;
        void Run();
        T GetValue();

    private:
        std::function<T()> m_func;
        std::promise<T> m_promise;
    };
    
    class FunctionTask : public Task
    {
    public:
        FunctionTask(std::function<void()> func_);
        virtual ~FunctionTask() = default;
        void Run();
    private:
        std::function<void()> m_func;
    };
    
    enum Priority {LOW, NORMAL, HIGH};
    
    void Add(std::shared_ptr<Task> task_, Priority priority_);
    void SetNumOfThreads(size_t n_threads_);
    void Pause();
    void Resume();
    void Stop();

private:
    explicit ThreadPool(size_t n_thread_ = std::thread::hardware_concurrency());
    friend class Singleton<ThreadPool, size_t>;
    friend class Singleton<ThreadPool>;

    using p_task = std::pair<std::shared_ptr<Task>, Priority>;

    const Priority MOST_LOW = static_cast<Priority> (LOW - 1);
    const Priority MOST_HIGH = static_cast<Priority> (HIGH + 1);

    bool m_is_paused;
    static thread_local bool is_running;

    mutable std::mutex m_lock;
    std::condition_variable m_cond;
    
    void WorkTask();
    void Stop(Priority p_);
    void AddThreads(size_t n_threads_);
    void ReduseThreads(size_t n_threads_);
    void Pause(size_t n_thread_);

    struct Compare
    {
        bool operator()(const p_task& p1, const p_task& p2)
        {
            return p1.second < p2.second;
        }
    };

    std::unordered_map<std::thread::id, std::thread> m_threads;
    WQueue <p_task, PriorityQueue <p_task, std::vector<p_task>, Compare > > m_wqueue;

    /*helper tasks*/
    static void StopTask();
    static std::thread::id DeletTask();
    void PauseTask();
};

template <typename T>
ThreadPool::FutureTask<T>::FutureTask(std::function<T()> func_) : m_func(func_)
{
    //empty
}

template <typename T>
void ThreadPool::FutureTask<T>::Run()
{
    m_promise.set_value(m_func());
}

template <typename T>
T ThreadPool::FutureTask<T>::GetValue()
{
    return m_promise.get_future().get();
}

#endif /*__RD1556_THREADPOOL_HPP__*/