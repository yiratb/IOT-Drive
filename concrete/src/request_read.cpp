#include <iostream> //std::cout

#include "request_read.hpp"

RequestRead::RequestRead(UID uid_, MessageType key_, uint32_t len_, uint64_t from_) 
            : MessageBase(uid_, key_), m_len(len_), m_from(from_)
{
}

char *RequestRead::ToBuffer(char *message_buff_)
{
    *(size_t*)message_buff_ = GetBufferLength();
    message_buff_ += sizeof(size_t);
    
    message_buff_ = MessageBase::ToBuffer(message_buff_);

    *(uint32_t*)message_buff_ = m_len;
    message_buff_ += sizeof(uint32_t);

    *(uint64_t*)message_buff_ = m_from;
    message_buff_ += sizeof(uint64_t);

    return message_buff_;
}

char *RequestRead::FromBuffer(char *message_buff_)
{
    message_buff_ = MessageBase::FromBuffer(message_buff_);

    m_len = *(uint32_t*)message_buff_;
    message_buff_ += sizeof(uint32_t);

    m_from = *(uint64_t*)message_buff_;
    message_buff_ += sizeof(uint64_t);

    return message_buff_;
}

size_t RequestRead::GetBufferLength()
{
    return MessageBase::GetBufferLength() + sizeof(uint32_t) + sizeof(uint64_t);
}

uint32_t RequestRead::GetLen() const
{
    return m_len;
}

uint64_t RequestRead::GetFrom() const
{
    return m_from;
}

void RequestRead::PrintData()
{
    std::cout <<"Len " << m_len  << std::endl;
    std::cout <<"From " << m_from  << std::endl;
}