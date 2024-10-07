#include <iostream> //std::cout
#include <cstring> //strcpy

#include "request_write.hpp"

RequestWrite::RequestWrite(UID uid_, MessageType key_, uint32_t len_, uint64_t from_, char *buffer_)
    : MessageBase(uid_, key_), m_len(len_), m_from(from_), m_buffer(new char[len_])
{
    memcpy(m_buffer, buffer_,m_len);
}

RequestWrite::~RequestWrite()
{
    delete[] m_buffer;
    m_buffer = nullptr;
}

char *RequestWrite::ToBuffer(char *message_buff_)
{
    *(size_t*)message_buff_ = GetBufferLength();
    message_buff_ += sizeof(size_t);
    
    message_buff_ = MessageBase::ToBuffer(message_buff_);

    *(uint32_t*)message_buff_ = m_len;
    message_buff_ += sizeof(uint32_t);

    *(uint64_t*)message_buff_ = m_from;
    message_buff_ += sizeof(uint64_t);

    memcpy(message_buff_, m_buffer, m_len);
    message_buff_ += m_len;

    return message_buff_;
}

char *RequestWrite::FromBuffer(char *message_buff_)
{
    message_buff_ = MessageBase::FromBuffer(message_buff_);

    m_len = *(uint32_t*)message_buff_;
    message_buff_ += sizeof(uint32_t);

    m_from = *(uint64_t*)message_buff_;
    message_buff_ += sizeof(uint64_t);

    m_buffer = new char[m_len];
    memcpy(m_buffer, message_buff_, m_len);
    message_buff_ += m_len;

    return message_buff_;
}

size_t RequestWrite::GetBufferLength()
{
    return MessageBase::GetBufferLength() + sizeof(uint32_t) + sizeof(uint64_t) + m_len;
}

void RequestWrite::PrintData()
{
    std::cout <<"Buffer (char*) " << m_buffer  << std::endl;
    std::cout <<"Len " << m_len  << std::endl;
    std::cout <<"From " << m_from  << std::endl;
}

uint32_t RequestWrite::GetLen() const
{
    return m_len;
}

uint64_t RequestWrite::GetFrom() const
{
    return m_from;
}

char *RequestWrite::GetBuffer()
{
    return m_buffer;
}
