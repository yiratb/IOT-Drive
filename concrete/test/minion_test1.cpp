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

const int PORT = 4000;
const std::string IP = "127.0.0.1";

int OpenFile();

// Debug:
using namespace std;

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
    int fd = open("minion_storage1", O_RDWR | O_CREAT, 0644);
    if (fd == -1) 
    {
        throw std::runtime_error("Failed to open the file!\n");
    }

    const size_t size = 4 * 1024 * 1024; // 4 MB

    // Set the file size to 4 MB
    if (ftruncate(fd, size) == -1) {
        close(fd);
        throw std::runtime_error("Failed to set the file size!\n");
    }

    // Optional: Fill the file with zeros if needed
    const size_t chunk_size = 4096; // Write in 4KB chunks
    char buffer[chunk_size];
    memset(buffer, 0, chunk_size);

    // Debug:
    // Changed "written" type from off_t to size_t
    for (size_t written = 0; written < size; written += chunk_size) 
    {
        if (write(fd, buffer, chunk_size) != chunk_size) 
        {
            close(fd);
            throw std::runtime_error("Failed to write zeros to the file!\n");
        }
    }

    return fd;
}
