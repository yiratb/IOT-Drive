#include <iostream>

#include "cpf.hpp"
#include <async_injection.hpp>

//using p_recipes = std::pair<int, std::function<std::shared_ptr<ICommand>>>;
//using t_listened_inputs = std::tuple<int, Reactor::event_type, IInputProxy>;

std::string CPF::path("logger_cpf");

// Debig:
using namespace std;

CPF::CPF(const std::vector< p_recipes > &recipes_, 
        const std::vector < t_listened_inputs >& listened_inputs_, const std::string& plugins_path_)
    : m_dir_monitor(DirMonitor(plugins_path_))
{
    // Debug:
    cout<<"CPF() 1" << endl;
    cout<<"path: " << path << endl;

    Logger *logger = Singleton<Logger, const std::string&>::GetInstance(path);

    // Debug:
    cout<<"CPF() 2" << endl;

    auto factory = Singleton<Factory<ICommand, int> >::GetInstance();

    for (const auto& recipe : recipes_) 
    {
        factory->Add(recipe.first, recipe.second);
    }
    
    for (const auto& iter : listened_inputs_)
    {
        logger->Log("CPF: Add to reactor", Logger::INFO);
        std::cout << "CPF: Add to reactor" << std::endl;

        m_reactor.Add(std::get<0>(iter), std::get<1>(iter), 
            std::bind(&CPF::CallBack, this, std::get<2>(iter)));
    }

    m_dir_monitor.GetDispatcher().Subscribe(&dll_loader);
    m_dir_monitor.StartMonitor();

    logger->Log("CPF: End of Ctor", Logger::INFO);
}

void CPF::Run()
{
    Logger *logger = Singleton<Logger, const std::string&>::GetInstance(path);
    logger->Log("CPF: Start Run \n", Logger::INFO);

    // Debug:
    std::cout << "Before Reactor::Run()\n";

    m_reactor.Run();
}

void CPF::Stop()
{
    Logger *logger = Singleton<Logger, const std::string&>::GetInstance(path);
    m_reactor.Stop();
    logger->Log("CPF: Stop", Logger::INFO);
}

void CPF::CallBack(std::shared_ptr<IInputProxy> input_proxy)
{
    Logger *logger = Singleton<Logger, const std::string&>::GetInstance(path);
    ThreadPool *tp = Singleton<ThreadPool>::GetInstance();
    logger->Log("CPF: Call to callback" ,Logger::INFO);

    std::shared_ptr<IKeyTask> ikey_task = input_proxy->GetTask();
    if (nullptr != ikey_task)
    {
        int key = ikey_task->GetKey();

        std::shared_ptr<TPTask> new_task = std::make_shared<TPTask>(key, ikey_task);

        tp->Add(new_task, ThreadPool::NORMAL);
    }
    
}

CPF::TPTask::TPTask(int key_, std::shared_ptr<IKeyTask> key_task_) : m_key(key_), m_key_task(key_task_)
{ }

void CPF::TPTask::Run()
{
    Logger *logger = Singleton<Logger, const std::string&>::GetInstance(CPF::path);
    logger->Log("CPF: Run the TPTask", Logger::INFO);

    auto factory = Singleton<Factory<ICommand, int> >::GetInstance();
    auto command = factory->Create(m_key);
    logger->Log("CPF: Run ICommand", Logger::INFO);

    ICommand::p_async_response async_response = command->Run(m_key_task);
    if (nullptr != async_response.first)
    {
        AsyncInjection *async_injection = new AsyncInjection(async_response.first,
                                                     async_response.second);
        (void)async_injection;
    }
}