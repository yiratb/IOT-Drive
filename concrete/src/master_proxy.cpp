#include "master_proxy.hpp"
#include "request_write.hpp"
#include "request_read.hpp"
#include "minion_key_task.hpp"
#include "reply_write.hpp"
#include "reply_read.hpp"
#include "logger.hpp"
#include "ip.hpp"
#include "factory.hpp"
#include <unistd.h>

#include <iostream>
using namespace std;

const size_t LEN_DATA = 10 * 1024;

MasterProxy::MasterProxy()
{
    m_ip = "127.0.0.1";
    m_port = 7000;
    
    m_send_socket_fd = Init(&m_servaddr, m_ip.c_str(), SOCK_DGRAM, m_port);

    /* connect to server */ 
	if(connect(m_send_socket_fd, (struct sockaddr *)&m_servaddr, sizeof(m_servaddr)) < 0) 
	{ 
		throw std::runtime_error("Connect Failed : MinionProxy\n");
    }
}

std::shared_ptr<IKeyTask> MasterProxy::GetTask()
{
    cout << "MasterProxy::GetTask()" << endl;

    Logger *logger = Singleton<Logger, const std::string&>::GetInstance("logger_minion");
    logger->Log("MasterProxy::GetTask" ,Logger::INFO);

    /* //recive the len and the type
    size_t len_buffer = sizeof(size_t) + sizeof(int);
    char *contain_len_buff = new char[len_buffer];
    int len = recv(m_listen_socket_fd, contain_len_buff, len_buffer, 0);
    if (-1 == len)
    {
        delete[] contain_len_buff;
        throw std::runtime_error("Failed to receive data length and type");
    }
    
    // Extract the length of the message
    size_t len_data = *(size_t *)contain_len_buff;
    std::cout << "Len Data " << len_data << std::endl;

    // Extract the type of the message
    contain_len_buff += sizeof(size_t);
    int type = *(int *)contain_len_buff;
    std::cout << "MasterProxy: Type" << type << std::endl; */

    // Read the actual data
    char *data_buffer = new char[LEN_DATA];
    int len2 = recv(m_listen_socket_fd, data_buffer, LEN_DATA, 0);
    if (-1 == len2)
    {
        //delete[] contain_len_buff;
        delete[] data_buffer;
        std::cout << "Failed to receive data of the message" << std::endl;
        throw std::runtime_error("Failed to receive data of the message");
    }

    MessageBase::MessageType type = *((MessageBase::MessageType *)(data_buffer + sizeof(size_t)));

    auto factory = Singleton<Factory<MessageBase, MessageBase::MessageType> >::GetInstance();
    auto request = factory->Create(type);

    request->FromBuffer(data_buffer);

    std::shared_ptr<MinionKeyTask> minion_key_task;

    if (MessageBase::MessageType::REQUEST_WRITE == type)
    {
        std::shared_ptr<RequestWrite> request_write = std::dynamic_pointer_cast<RequestWrite>(request);

        minion_key_task = std::make_shared<MinionKeyTask>(
            MinionUtility::CommandKey::MINION_CMD_WRITE,
            request_write->GetLen(),
            request_write->GetFrom(),
            request_write->GetBuffer(),
            request_write->GetUid()
        );
    }
    else if(MessageBase::MessageType::REQUEST_READ == type)
    {
        std::shared_ptr<RequestRead> request_read = std::dynamic_pointer_cast<RequestRead>(request);

        minion_key_task = std::make_shared<MinionKeyTask>(
            MinionUtility::CommandKey::MINION_CMD_READ,
            request_read->GetLen(),
            request_read->GetFrom(),
            nullptr,
            request_read->GetUid()
        );
    }

    //delete[] contain_len_buff;
    delete[] data_buffer;

    return minion_key_task;
}

void MasterProxy::SendResponseWrite(MinionUtility::Status status, std::shared_ptr<IKeyTask> task_)
{
    std::shared_ptr<MinionKeyTask> minion_key_task = std::dynamic_pointer_cast<MinionKeyTask>(task_);

    minion_key_task->GetData().m_uid;
    ReplyWrite reply_write(minion_key_task->GetData().m_uid,
                           MessageBase::MessageType::REPLY_WRITE,
                           status);

    size_t len_network_buffer = reply_write.GetBufferLength() + sizeof(size_t) + sizeof(int);
    char *network_buffer = new char[len_network_buffer];     
    reply_write.ToBuffer(network_buffer);    

    ssize_t num_bytes = send(m_send_socket_fd, network_buffer, len_network_buffer, 0);
    if (-1 == num_bytes)
    {
        throw std::runtime_error("Send Failed : MasterProxy::Write\n");
    }
    reply_write.PrintData();
}

void MasterProxy::SendResponseRead(MinionUtility::Status status, std::shared_ptr<IKeyTask> task_)
{
    std::cout << "Status " << status << std::endl;
    std::cout << "MasterProxy Send Response Read" << std::endl;
    
    std::shared_ptr<MinionKeyTask> minion_key_task = std::dynamic_pointer_cast<MinionKeyTask>(task_);
    
    minion_key_task->GetData().m_uid;
    ReplyRead reply_read(minion_key_task->GetData().m_uid,
                           MessageBase::MessageType::REPLY_READ,
                           minion_key_task->GetData().m_len,
                           minion_key_task->GetData().m_from,
                           minion_key_task->GetData().m_chunk,
                           status);
    
    size_t len_network_buffer = reply_read.GetBufferLength() + sizeof(size_t) + sizeof(int);
    char *network_buffer = new char[len_network_buffer];     
    reply_read.ToBuffer(network_buffer);            

    ssize_t num_bytes = send(m_send_socket_fd, network_buffer, len_network_buffer, 0);
    if (-1 == num_bytes)
    {
        throw std::runtime_error("Send Failed : MasterProxy::Read\n");
    }
    reply_read.PrintData();
    std::cout << "Finish send Response Read (*)" << std::endl;
}

void MasterProxy::SetFd(int socket_fd_)
{
    m_listen_socket_fd = socket_fd_;
}
