#include <cstring> //strcpy

#include "reply_read.hpp"

ReplyRead::ReplyRead(UID uid_, MessageType key_, uint32_t len_, uint64_t from_, 
char *buffer_, MinionUtility::Status status) : MessageBase(uid_, key_) , 
        m_len(len_), m_from(from_), m_buffer(new char[len_]), m_status(status)
{
    memcpy(m_buffer, buffer_, len_);
}

ReplyRead::~ReplyRead()
{
    delete[] m_buffer;
    m_buffer = nullptr;
}

char *ReplyRead::ToBuffer(char *message_buff_)
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

    *(MinionUtility::Status *)message_buff_ = m_status;
    message_buff_ += sizeof(MinionUtility::Status);

    return message_buff_;
}

char *ReplyRead::FromBuffer(char *message_buff_)
{
    message_buff_ = MessageBase::FromBuffer(message_buff_);

    m_len = *(uint32_t*)message_buff_;
    message_buff_ += sizeof(uint32_t);

    m_from = *(uint64_t*)message_buff_;
    message_buff_ += sizeof(uint64_t);

    m_buffer = new char[m_len];
    memcpy(m_buffer, message_buff_, m_len);
    message_buff_ += m_len;

    m_status = *(MinionUtility::Status *)message_buff_;
    message_buff_ += sizeof(MinionUtility::Status);

    return message_buff_;
}

size_t ReplyRead::GetBufferLength()
{
    return MessageBase::GetBufferLength() + sizeof(uint32_t) + sizeof(uint64_t)
                     + m_len + sizeof(MinionUtility::Status);
}

void ReplyRead::PrintData()
{
    std::cout <<"Buffer (char*)" << m_buffer  << std::endl;
    std::cout <<"Len " << m_len  << std::endl;
    std::cout <<"From " << m_from  << std::endl;
    std::cout <<"status " << m_status  << std::endl;
}

MinionUtility::Status ReplyRead::GetStatus() const
{
    return m_status;
}

char *ReplyRead::GetBuffer() const
{
    return m_buffer;
}

uint32_t ReplyRead::GetLen() const
{
    return m_len;
}
