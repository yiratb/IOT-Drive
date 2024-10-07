#ifndef ILRD_RD1556_CPF_HPP
#define ILRD_RD1556_CPF_HPP

#include <map> //std::map
#include <functional> //std::function
#include <memory> //std::shared_pointer
 #include <tuple>

#include "icommand.hpp" //ICommand
#include "iinput_proxy.hpp" //IInputProxy
#include "ikey_task.hpp" //IKeyTask
#include "reactor.hpp" //event_type
#include "thread_pool.hpp" //ThreadPool
#include "factory.hpp" //factory
#include "logger.hpp" //logger
#include "dir_monitor.hpp" //dirmonitor

class CPF
{
public:
    using event = Reactor::event_type;
    static constexpr event READ = Reactor::READ;
    static constexpr event WRITE = Reactor::WRITE;
    static constexpr event ERROR = Reactor::ERROR; 


    using p_recipes = std::pair<int, std::function<std::shared_ptr<ICommand>()>>;
    using t_listened_inputs = std::tuple<int, Reactor::event_type, std::shared_ptr<IInputProxy> >;
    
    explicit CPF(const std::vector< p_recipes > &recipes_, 
        const std::vector < t_listened_inputs >& listened_inputs_, const std::string& plugins_path_);

    ~CPF() = default;
    void Run();
    void Stop();
    static std::string path;

private:
    Reactor m_reactor;
    DirMonitor m_dir_monitor;
    DllLoader dll_loader;

    void CallBack(std::shared_ptr<IInputProxy> input_proxy);

    class TPTask : public ThreadPool::Task
    {
    public:
        TPTask(int key_, std::shared_ptr<IKeyTask> key_task_);
        ~TPTask() = default;
        void Run();
    private:
        int m_key;
        std::shared_ptr<IKeyTask> m_key_task;
    };
    
};

#endif //ILRD_RD1556_CPF_HPP