#include <iostream> //cout
#include <unistd.h> //open
#include <fcntl.h> //open

#include "cpf.hpp"
#include "plugins_test.hpp"
#include <istream>
#include <cstring>

class IKeyTaskRead : public IKeyTask
{
public:
    IKeyTaskRead(char* buffer_,size_t length)
    {
        m_buffer = new char[length];
        std::memcpy(m_buffer,buffer_,length);
    }

    ~IKeyTaskRead(){delete m_buffer;};
    int GetKey()
    { 
        return 1; 
    }
    char* m_buffer;
};

class ICommandEcho : public ICommand
{
public:
    ICommandEcho() = default;
    ~ICommandEcho() = default;
    void Run(std::shared_ptr<IKeyTask> task_) 
    {
        Logger *logger = Singleton<Logger, const std::string&>::GetInstance(CPF::path);
        logger->Log("ICommandEcho Run" ,Logger::INFO);

        std::shared_ptr<IKeyTaskRead> task = std::dynamic_pointer_cast<IKeyTaskRead>(task_);
        std::cout << "READ " << task->m_buffer << std::endl;
    }
};

class IInputProxyRead : public IInputProxy
{
public:
    IInputProxyRead(int fd_) : m_fd(fd_) {}
    ~IInputProxyRead() = default;
    std::shared_ptr<IKeyTask> GetTask()
    {
        Logger *logger = Singleton<Logger, const std::string&>::GetInstance(CPF::path);
        const auto MAX_SIZE = 1024;
        char buffer[MAX_SIZE] = {0};

        read(m_fd, buffer, 1024);
        logger->Log("IInputProxyRead Read BUffer" ,Logger::INFO);

        return std::make_shared<IKeyTaskRead>(buffer);
    }
    int m_fd;
};


//using p_recipes = std::pair<int, std::function<std::shared_ptr<ICommand>()>>;
//using t_listened_inputs = std::tuple<int, Reactor::event_type, std::shared_ptr<IInputProxy> >;

int main()
{
    std::vector<CPF::p_recipes> recipes;
    recipes.push_back(CPF::p_recipes(1, []() { return std::make_shared<ICommandEcho>(); }));
    recipes.push_back(CPF::p_recipes(1, []() { return std::make_shared<ICommandEcho>(); }));

    int fds1[2] = {0};
    pipe(fds1);

    int fds2[2] = {0};
    pipe(fds2);
    
    std::shared_ptr<IInputProxy> input_proxy1 = std::make_shared<IInputProxyRead>(fds1[0]);
    std::shared_ptr<IInputProxy> input_proxy2 = std::make_shared<IInputProxyRead>(fds2[0]);

    std::vector<CPF::t_listened_inputs> listened_inputs;
    listened_inputs.push_back(CPF::t_listened_inputs(fds1[0], CPF::WRITE, input_proxy1));
    listened_inputs.push_back(CPF::t_listened_inputs(fds2[0], CPF::WRITE, input_proxy2));

    std::string plugins_dir = "/home/gil/Downloads/yirat.romano-master-projects-final_project/projects/final_project/framework/plugins";


    CPF cpf(recipes, listened_inputs, plugins_dir);

    std::thread th(std::bind(&CPF::Run, &cpf));

    char buffer1[1024] = "hello";
    char buffer2[1024] = "world!";
    write(fds1[1], buffer1, 1024);
    write(fds2[1], buffer2, 1024);

    std::this_thread::sleep_for(std::chrono::seconds(7));
    cpf.Stop();
    if (th.joinable())
    {
        th.join();
    }

    try
    {
        Factory<Base, size_t> *f = Singleton<Factory<Base, size_t>>::GetInstance();
        std::cout << "Address in main" << f << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(10)); // Run for 10 seconds for testing

        std::shared_ptr<Base> obj = f->Create(3);
        obj->Print();

    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
