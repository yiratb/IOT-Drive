#include <unistd.h> //close
#include <iostream> //std::cout
#include <netinet/in.h> //recv
#include <memory>

#include "response_input_proxy.hpp"
#include "message_base.hpp"
#include "factory.hpp"
#include "reply_read.hpp"
#include "reply_write.hpp"
#include "response_manager.hpp"
#include "singleton.hpp"
#include "nbd_proxy.hpp"
#include "logger.hpp"

const size_t LEN_DATA = 10 * 1024;

ResponseInputProxy::ResponseInputProxy(int fd_) : m_fd(fd_)
{
}


std::shared_ptr<IKeyTask> ResponseInputProxy::GetTask()
{
    Logger *logger = Singleton<Logger, const std::string&>::GetInstance("logger_cpf");

    logger->Log("Response Start GetTask" ,Logger::INFO);

    char *data_buffer = new char[LEN_DATA];
    int len2 = recv(m_fd, data_buffer, LEN_DATA, 0);
    if (-1 == len2)
    {
        delete[] data_buffer;
        std::cout << "Failed to receive data of the message" << std::endl;
        throw std::runtime_error("Failed to receive data of the message");
    }

    MessageBase::MessageType type = *((MessageBase::MessageType *)(data_buffer + sizeof(size_t)));

    auto factory = Singleton<Factory<MessageBase, MessageBase::MessageType> >::GetInstance();
    auto reply = factory->Create(type);

    reply->FromBuffer(data_buffer);

    ResponsManager *response_manager =  Singleton<ResponsManager>::GetInstance();

    if (MessageBase::MessageType::REPLY_WRITE == type)
    {
        std::shared_ptr<ReplyWrite> reply_write = std::dynamic_pointer_cast<ReplyWrite>(reply);

        std::shared_ptr<NBDKeyTask> nbd_key_task = (response_manager->GetResponseData(reply_write->GetUid())).second;
        
        //m_nbd.WriteAll(nbd_fd, (char *)&(nbd_key_task->GetData().m_reply), sizeof(nbd_reply));
        NbdProxy *nbd_proxy = Singleton<NbdProxy>::GetInstance();
        nbd_proxy->SendResponse((response_manager->
                                GetResponseData(reply_write->GetUid())).second,
                                nullptr);

        response_manager->SetStatus(reply_write->GetUid(), reply_write->GetStatus());

    }
    else if(MessageBase::MessageType::REPLY_READ == type)
    {
        std::cout << "ResponseInputProxy::GetTask() Read" << std::endl;


        std::shared_ptr<ReplyRead> reply_read = std::dynamic_pointer_cast<ReplyRead>(reply);

        std::shared_ptr<NBDKeyTask> nbd_key_task = (response_manager->GetResponseData(reply_read->GetUid())).second;
        

        /* m_nbd.WriteAll(nbd_fd, (char *)&(nbd_key_task->GetData().m_reply), sizeof(nbd_reply));
        m_nbd.WriteAll(nbd_fd, reply_read->GetBuffer(), nbd_key_task->GetData().m_len); */
        
        NbdProxy *nbd_proxy = Singleton<NbdProxy>::GetInstance();

        logger->Log("Response INputProxy Before SendResponse " ,Logger::INFO);
        nbd_proxy->SendResponse((response_manager->
                         GetResponseData(reply_read->GetUid())).second,
                         reply_read->GetBuffer());

        logger->Log("Response INputProxy After SendResponse " ,Logger::INFO);

        response_manager->SetStatus(reply_read->GetUid(),reply_read->GetStatus());

        std::cout << "ResponseInputProxy::GetTask() Read end" << std::endl;        
    }
    //std::cout << "End ResponseInputProxy::GetTask() before delete[]" << std::endl;

    delete[] data_buffer;

    return nullptr;
}