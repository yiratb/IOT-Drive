#include <iostream> //cout
#include <unistd.h> //open
#include <fcntl.h> //open
#include <sys/socket.h>
#include <netinet/in.h> //sockaddr_in
#include <linux/nbd.h> //NBD_CMD_WRITE
#include <cstring> //strlen
#include <chrono>
#include <thread>

#include "cpf.hpp"
#include "nbd_command_read.hpp"
#include "nbd_command_write.hpp"
#include "nbd_command_trim.hpp"
#include "nbd_command_disc.hpp"
#include "nbd_command_flush.hpp"
#include "nbd_proxy.hpp"
#include "response_input_proxy.hpp"
#include "ip.hpp"
#include "message_base.hpp"
#include "request_read.hpp"
#include "request_write.hpp"
#include "reply_read.hpp"
#include "reply_write.hpp"

using namespace std;

int CreateSocket();

int main(int argc, char const *argv[])
{
    try
    {
        std::string plugins_dir = "/home/yiratromano/git/projects/final_project/framework/plugins";

        std::string nbd_path = argv[1];
        size_t size = 4194304;//4 * 1024 * 1024; 
        NBD nbd(size, nbd_path);
        nbd.ListenNbd();
        (void)argc;

        /*******************************Init Factory**************************/
        auto factory = Singleton<Factory<MessageBase, MessageBase::MessageType> >::GetInstance();
        factory->Add(MessageBase::MessageType::REQUEST_READ, []() { return std::make_shared<RequestRead>(); });
        factory->Add(MessageBase::MessageType::REQUEST_WRITE, []() { return std::make_shared<RequestWrite>(); });
        factory->Add(MessageBase::MessageType::REPLY_WRITE, []() { return std::make_shared<ReplyWrite>(); });
        factory->Add(MessageBase::MessageType::REPLY_READ, []() { return std::make_shared<ReplyRead>(); });

        /*******************************Init CPF**************************/
        int fd_nbd = nbd.GetFd();
        std::cout << "Fd_nbd in main " << fd_nbd << std::endl; 

        int listen_minion_fd = CreateSocket();

        std::vector<CPF::p_recipes> recipes;
        recipes.push_back(CPF::p_recipes(NBD_CMD_WRITE, []() { return std::make_shared<NBDCommandWrite>(); }));
        recipes.push_back(CPF::p_recipes(NBD_CMD_READ, []() { return std::make_shared<NBDCommandRead>(); }));
        recipes.push_back(CPF::p_recipes(NBD_CMD_FLUSH, []() { return std::make_shared<NBDCommandFlush>(); }));
        recipes.push_back(CPF::p_recipes(NBD_CMD_TRIM, []() { return std::make_shared<NBDCommandTrim>(); }));
        recipes.push_back(CPF::p_recipes(NBD_CMD_DISC, []() { return std::make_shared<NBDCommandDisc>(); }));

        NbdProxy *nbd_input_proxy = Singleton<NbdProxy>::GetInstance();
        nbd_input_proxy->SetFd(fd_nbd);
        nbd_input_proxy->SetNBD(&nbd);
        std::shared_ptr<IInputProxy> shared_ptr_nbd_proxy(nbd_input_proxy);

        std::shared_ptr<IInputProxy> response_input_proxy = std::make_shared<ResponseInputProxy>(listen_minion_fd);

        std::vector<CPF::t_listened_inputs> listened_inputs;
        //listened_inputs.push_back(CPF::t_listened_inputs(fd_nbd, CPF::WRITE, shared_ptr_nbd_proxy));
        listened_inputs.push_back(CPF::t_listened_inputs(fd_nbd, CPF::READ, shared_ptr_nbd_proxy));

        //listened_inputs.push_back(CPF::t_listened_inputs(listen_minion_fd, CPF::WRITE, response_input_proxy));
        listened_inputs.push_back(CPF::t_listened_inputs(listen_minion_fd, CPF::READ, response_input_proxy));

        /*******************************Run CPF**************************/
        CPF cpf(recipes, listened_inputs, plugins_dir);
        cpf.Run();
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
