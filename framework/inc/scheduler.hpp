#ifndef ILRD_RD1556_SCHED_HPP
#define ILRD_RD1556_SCHED_HPP

#include <memory> // shared_ptr
#include <chrono> // std::chrono::milliseconds
#include <vector> // std::vector
#include <mutex> // std::mutex
#include <ctime> // std::time_t
#include <queue> // std::priority_queue
#include <vector> // std::vector

#define ALLOWED_SINGLETON
#include "singleton.hpp"
#undef ALLOWED_SINGLETON

/**
 * Scheduler
 * A class that manages and schedules tasks to be executed at specific times.
 *
 * The Scheduler class uses a priority queue to manage tasks and a timer to execute them.
 * It ensures that tasks are executed at their scheduled time, using a background thread
 * to handle task execution.
 */
class Scheduler
{
public:

    /**
     * ITask
     * An interface for tasks that can be scheduled by the Scheduler.
     *
     * Derived classes must implement the Execute method to define the task's behavior.
     */
    class ITask
    {
    public:
        virtual void Execute() = 0;
        virtual ~ITask() = default;
    };

    Scheduler(const Scheduler& other_) = delete;
    Scheduler& operator=(const Scheduler& other_) = delete;

    /**
     * @brief Adds a task to the scheduler.
     *
     * @param task_ A shared pointer to the task to be added.
     * @param time_to_exec_ The delay in milliseconds after which the task should be executed.
     */
    void Add(std::shared_ptr<ITask> task_, std::chrono::milliseconds time_to_exec_);

private:
    friend Singleton<Scheduler>;
    explicit Scheduler();
    ~Scheduler();

    /**
     * @brief Timer handler function that is called when the timer expires.
     *
     * @param sv A union containing a pointer to the Scheduler instance.
     */
    static void TimerHandler(union sigval sv);

    /**
     * @brief Updates the timer to the next scheduled task's execution time.
     */
    void UpdateTime();

    using p_key = std::pair<std::time_t, std::shared_ptr<ITask>>;

    /**
     * @struct TaskCompare
     * @brief A functor for comparing tasks based on their scheduled execution time.
     */
    struct TaskCompare
    {
        bool operator()(const p_key& a, const p_key& b)
        {
            return a.first > b.first;
        }
    };

    /**
    * converting a time_point to milliseconds
    */
    time_t ConvertToDuration(const std::chrono::time_point<std::chrono::system_clock> time_point);

    /**
    * return time now
    */
    std::chrono::time_point<std::chrono::system_clock> time_now();

    std::priority_queue<p_key, std::vector<p_key>, TaskCompare> m_tasks; ///< Priority queue for managing tasks.
    timer_t m_timer_id; ///< Timer ID for managing timer events.
    std::mutex m_mutex_; ///< Mutex for synchronizing access to the task queue.
};

#endif // ILRD_RD1556_SCHED_HPP
