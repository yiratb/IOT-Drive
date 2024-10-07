#include <unistd.h> //close

#include "minion_proxy.hpp"
#include "response_manager.hpp"
#include "logger.hpp"
#include "request_write.hpp"
#include "request_read.hpp"
#include "ip.hpp"
#include "minion_utility.hpp"

#include <iostream>
using namespace std;

MinionProxy::MinionProxy(std::string ip, size_t port) : m_ip(ip), m_port(port)
{
	m_sockfd = Init(&m_servaddr, m_ip.c_str(), SOCK_DGRAM, m_port);

    /* connect to server */ 
	if(connect(m_sockfd, (struct sockaddr *)&m_servaddr, sizeof(m_servaddr)) < 0) 
	{ 
		throw std::runtime_error("Connect Failed : MinionProxy\n");
    }
}

MinionProxy::~MinionProxy()
{
	close(m_sockfd);
}

UID MinionProxy::Read(uint64_t offset, std::shared_ptr<NBDKeyTask> task)
{
    Logger *logger = Singleton<Logger, const std::string&>::GetInstance("logger_cpf");
    logger->Log("MinionProxy read with offset " + std::to_string(offset) ,Logger::INFO);
    logger->Log("and len " + std::to_string(task->GetData().m_len) ,Logger::INFO);

    UID uid;
    Singleton<ResponsManager>::GetInstance()->Add(
                uid,
                ResponsManager::p_data(MinionUtility::NO_RESPONSE_YET, task));

    RequestRead request_read(uid, MessageBase::REQUEST_READ, task->GetData().m_len, offset);

    size_t len_network_buffer = request_read.GetBufferLength() + sizeof(size_t) + sizeof(int);
    char *network_buffer = new char[len_network_buffer];
    request_read.ToBuffer(network_buffer);

    logger->Log("MinionProxy Before send " ,Logger::INFO);

    ssize_t num_bytes = send(m_sockfd, network_buffer, len_network_buffer, 0);
    if (-1 == num_bytes)
    {
        throw std::runtime_error("Send Failed : MinionProxy::Read\n");
    }

    logger->Log("MinionProxy After send " ,Logger::INFO);
    return uid;
}

UID MinionProxy::Write(uint64_t offset, std::shared_ptr<NBDKeyTask> task)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    Logger *logger = Singleton<Logger, const std::string&>::GetInstance("logger_cpf");
    logger->Log("MinionProxy write with offset " + std::to_string(offset) ,Logger::INFO);
    logger->Log("and len " + std::to_string(task->GetData().m_len) ,Logger::INFO);
    logger->Log("and buffer " ,Logger::INFO);
    logger->Log(task->GetData().m_chunk, Logger::INFO);

    UID uid;
    Singleton<ResponsManager>::GetInstance()->Add(
                uid,
                ResponsManager::p_data(MinionUtility::NO_RESPONSE_YET, task));

    RequestWrite request_write(uid, MessageBase::REQUEST_WRITE, task->GetData().m_len, offset, task->GetData().m_chunk);

    size_t len_network_buffer = request_write.GetBufferLength() + sizeof(size_t) + sizeof(int);
    char *network_buffer = new char[len_network_buffer];
    request_write.ToBuffer(network_buffer);
    
    ssize_t num_bytes;
    do
    {
        num_bytes = send(m_sockfd, network_buffer, len_network_buffer, 0);
        if (-1 == num_bytes)
        {
            string s = "!!!Send Failed : MinionProxy::Write. send() returned -1. errno is: ";
            s+=std::to_string(errno);
            s+=" len_network_buffer was: ";
            s+=std::to_string(len_network_buffer);
            s+="\n";
            cout << s << endl;
            cout << "errno was..." << errno << endl;
            sleep(1);

            logger->Log(s,Logger::ERROR);

        }
    }
    while (num_bytes == -1);

    return uid;
}