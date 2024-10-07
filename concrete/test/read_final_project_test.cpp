#include <iostream> //cout
#include <unistd.h> //open
#include <fcntl.h> //open
#include <sys/socket.h>
#include <netinet/in.h> //sockaddr_in
#include <cstring> //strlen
#include <linux/nbd.h>

#include "cpf.hpp"
#include "nbd_command_read.hpp"
#include "nbd_command_write.hpp"
#include "nbd_proxy.hpp"
#include "response_input_proxy.hpp"
#include "ip.hpp"
#include "message_base.hpp"
#include "request_read.hpp"
#include "request_write.hpp"
#include "reply_read.hpp"
#include "reply_write.hpp"

int CreateSocket();

int main()
{
    try
    {
        std::string plugins_dir = "/home/gil/Downloads/yirat.romano-master-projects-final_project/projects/final_project/framework/plugins";

        //std::string nbd_path =  "/dev/nbd0";
        //size_t size = 4 * 1024 * 1024;
        //NBD nbd(size, nbd_path);

        /*******************************Init Factory**************************/
        auto factory = Singleton<Factory<MessageBase, MessageBase::MessageType> >::GetInstance();
        factory->Add(MessageBase::MessageType::REQUEST_READ, []() { return std::make_shared<RequestRead>(); });
        factory->Add(MessageBase::MessageType::REQUEST_WRITE, []() { return std::make_shared<RequestWrite>(); });
        factory->Add(MessageBase::MessageType::REPLY_WRITE, []() { return std::make_shared<ReplyWrite>(); });
        factory->Add(MessageBase::MessageType::REPLY_READ, []() { return std::make_shared<ReplyRead>(); });

        /*******************************Init CPF**************************/
        int fds1[2] = {0};
        pipe(fds1);

        int listen_minion_fd = CreateSocket();

        std::vector<CPF::p_recipes> recipes;
        recipes.push_back(CPF::p_recipes(NBD_CMD_READ, []() { return std::make_shared<NBDCommandRead>(); }));

        std::shared_ptr<IInputProxy> input_proxy1 = std::make_shared<NbdProxy>(fds1[0]); //nbd
        std::shared_ptr<IInputProxy> response_input_proxy = std::make_shared<ResponseInputProxy>(listen_minion_fd);

        std::vector<CPF::t_listened_inputs> listened_inputs;
        listened_inputs.push_back(CPF::t_listened_inputs(fds1[0], CPF::WRITE, input_proxy1));
        listened_inputs.push_back(CPF::t_listened_inputs(listen_minion_fd, CPF::WRITE, response_input_proxy));

        /*******************************Run CPF**************************/
        CPF cpf(recipes, listened_inputs, plugins_dir);

        std::thread th(std::bind(&CPF::Run, &cpf));

        char buffer1[1024] = "hello";
        write(fds1[1], buffer1, strlen(buffer1));

        std::this_thread::sleep_for(std::chrono::seconds(7));
        cpf.Stop();
        if (th.joinable())
        {
            th.join();
        } 
        close(listen_minion_fd);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}

int CreateSocket()
{
    std::string ip = "127.0.0.1";
    int port = 7000;

    int listen_minion_fd; 
    struct sockaddr_in servaddr;

    listen_minion_fd = Init(&servaddr, ip.c_str(), SOCK_DGRAM, port);

    if (bind(listen_minion_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        throw std::runtime_error("MinionTest: bind Fail");
    }

    return listen_minion_fd;
}