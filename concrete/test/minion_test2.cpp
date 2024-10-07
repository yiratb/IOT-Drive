#include <iostream> //cout
#include <unistd.h> //open
#include <fcntl.h> //open
#include <linux/nbd.h> //nbd enum
#include <netinet/in.h> //sockaddr_in
#include <string> //strcpy
#include <string.h> //memcpy

#include "cpf.hpp"
#include "minion_command_read.hpp"
#include "minion_command_write.hpp"
#include "master_proxy.hpp"
#include "ip.hpp"
#include "message_base.hpp"
#include "request_read.hpp"
#include "request_write.hpp"
#include "minion_utility.hpp"
#include "reply_read.hpp"
#include "reply_write.hpp"

const int PORT = 4500;
const std::string IP = "127.0.0.1";

int OpenFile();

int main()
{
    try
    {
        std::string plugins_dir = "/home/yiratromano/git/projects/final_project/framework/plugins";

        /*******************************Open File**************************/
        int fd = OpenFile();

        MinionUtility *minion_utility = Singleton<MinionUtility>::GetInstance();
        minion_utility->SetFd(fd);

        /*******************************Open Socket**************************/
        int socket_fd; 
        struct sockaddr_in servaddr;

        socket_fd = Init(&servaddr, IP.c_str(), SOCK_DGRAM, PORT);

        if (bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        {
            throw std::runtime_error("MinionTest: bind Fail");
        }

        /*******************************Init Factory**************************/
        auto factory = Singleton<Factory<MessageBase, MessageBase::MessageType> >::GetInstance();
        factory->Add(MessageBase::MessageType::REQUEST_READ, []() { return std::make_shared<RequestRead>(); });
        factory->Add(MessageBase::MessageType::REQUEST_WRITE, []() { return std::make_shared<RequestWrite>(); });
        factory->Add(MessageBase::MessageType::REPLY_WRITE, []() { return std::make_shared<ReplyWrite>(); });
        factory->Add(MessageBase::MessageType::REPLY_READ, []() { return std::make_shared<ReplyRead>(); });

        /*******************************Init CPF**************************/
        MasterProxy *master_proxy = Singleton<MasterProxy>::GetInstance();
        master_proxy->SetFd(socket_fd);
        std::shared_ptr<IInputProxy> shared_ptr_proxy(master_proxy);

        std::vector<CPF::t_listened_inputs> listened_inputs;
        //listened_inputs.push_back(CPF::t_listened_inputs(socket_fd, CPF::WRITE, shared_ptr_proxy));
        listened_inputs.push_back(CPF::t_listened_inputs(socket_fd, CPF::READ, shared_ptr_proxy));


        std::vector<CPF::p_recipes> recipes;
        recipes.push_back(CPF::p_recipes(MinionUtility::CommandKey::MINION_CMD_WRITE, []() { return std::make_shared<MinionCommandWrite>(); }));
        recipes.push_back(CPF::p_recipes(MinionUtility::CommandKey::MINION_CMD_READ, []() { return std::make_shared<MinionCommandRead>(); }));
        CPF cpf(recipes, listened_inputs, plugins_dir);
        cpf.Run();

        close(socket_fd);
        close(fd);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}

int OpenFile()
{

     // Open the file using file descriptor
    int fd = open("minion_storage2", O_RDWR | O_CREAT, 0644);
    if (fd == -1) 
    {
        throw std::runtime_error("Failed to open the file!\n");
    }

    const size_t size = 4 * 1024 * 1024; // 4MB
    char buffer[4096]; // 4KB buffer
    memset(buffer, 0, sizeof(buffer));

    for (size_t written = 0; written < size; written += sizeof(buffer)) 
    {
        ssize_t result = write(fd, buffer, sizeof(buffer));
        if (result == -1) 
        {
            close(fd);
            throw std::runtime_error("Failed to write to the file!\n");
        }
    }

/*     lseek(fd, 0, SEEK_SET);
    
    ssize_t bytesW_written = write(fd, "hello\0",6);
    if (-1 == bytesW_written) 
    {
        throw std::runtime_error("Failed to open the file!\n");
    } */

    return fd;
}
