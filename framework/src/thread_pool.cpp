/****************************************************                                                                                              
 *  Written by: Yirat Romano                          
 *                                                  
 *  Reviewed by:                            
 *                                                  
 *  Completed on - 20/06/24                         
 *                                                  
 ****************************************************/

#include "thread_pool.hpp" /* thread_pool */

thread_local bool ThreadPool::is_running = true;

ThreadPool::ThreadPool(size_t n_thread_): m_is_paused(false)
{
    AddThreads(n_thread_);
}

ThreadPool::~ThreadPool()
{
    if (m_threads.size() > 0)
    {
        Stop(MOST_HIGH);
    }
}

void ThreadPool::Add(std::shared_ptr<ThreadPool::Task> task_, ThreadPool::Priority priority_)
{
    p_task new_task = {task_, priority_};

    m_wqueue.Push(new_task);
}

void ThreadPool::SetNumOfThreads(size_t n_thread_)
{
    size_t size = m_threads.size();

    if (n_thread_ > size)
    {
        if (m_is_paused)
        {
            Pause(n_thread_ - size);
        }

        AddThreads(n_thread_ - size);
    }
    else if(n_thread_ < size)
    {
        ReduseThreads(size - n_thread_);
    }
}

void ThreadPool::Pause()
{
    Pause(m_threads.size());
}

void ThreadPool::Pause(size_t n_thread_)
{
    m_is_paused = true;

    for (size_t i = 0; i < n_thread_ ; ++i)
    {
        Add(std::make_shared<ThreadPool::FunctionTask>(std::bind(&ThreadPool::PauseTask, this)), MOST_HIGH);
    }
}

void ThreadPool::Resume()
{
    m_is_paused = false;

    m_cond.notify_all();
}

void ThreadPool::Stop()
{
    Stop(MOST_LOW);  
}

/***************************PRIVATE METHOD**********************/

void ThreadPool::Stop(ThreadPool::Priority p_)
{
    Resume();

    for (size_t i = 0; i < m_threads.size() ; ++i)
    {
        Add(std::make_shared<ThreadPool::FunctionTask>(StopTask), p_);
    }
    
    for (auto &thread_pair : m_threads)
    {
        thread_pair.second.join();
    }

    m_threads.clear();
}

void ThreadPool::AddThreads(size_t n_thread_)
{
    size_t i = 0;
    
    for (i = 0; i < n_thread_; ++i)
    {
        std::thread thread(std::bind(&ThreadPool::WorkTask, this));
        m_threads[thread.get_id()] = std::move(thread);
    }
}

void ThreadPool::ReduseThreads(size_t n_thread_)
{
    size_t i = 0;
    std::thread::id tid;
    std::shared_ptr<ThreadPool::FutureTask<std::thread::id >> *tasks =
     new std::shared_ptr<ThreadPool::FutureTask<std::thread::id >> [n_thread_];

    for (i = 0; i < n_thread_; ++i)
    {
        tasks[i] = std::make_shared<ThreadPool::FutureTask<std::thread::id >>(DeletTask);
        Add(tasks[i], MOST_HIGH);
    }

    for (i = 0; i < n_thread_; ++i)
    {
        m_cond.notify_one();
    }
    
    for (i = 0; i < n_thread_; ++i)
    {
        tid = tasks[i]->GetValue();
        m_threads[tid].join();
        m_threads.erase(tid);
    }
    
    delete[] tasks;
}

void ThreadPool::WorkTask()
{
    p_task out_param;

    while (is_running)
    {
        m_wqueue.Pop(out_param);
        out_param.first->Run();
    }
}

/****************************IMP TASKS***************************** */

ThreadPool::FunctionTask::FunctionTask(std::function<void()> func_) : m_func(func_)
{
    //empty
}
void ThreadPool::FunctionTask::Run()
{
    m_func();
}

/****************************HELPER TASKS*****************************/

void ThreadPool::StopTask()
{
    is_running = false;
}

void ThreadPool::PauseTask()
{
    std::unique_lock<std::mutex> lock(m_lock);
    
    if (m_is_paused)
    {
        m_cond.wait(lock);
    }
}

std::thread::id ThreadPool::DeletTask()
{
    is_running = false;

    return std::this_thread::get_id();
}