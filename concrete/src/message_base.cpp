#include "message_base.hpp"

MessageBase::MessageBase(UID uid_, MessageType key_) : m_key(key_), m_uid(uid_)
{
}

char *MessageBase::ToBuffer(char *message_buff_)
{
    *(int*)message_buff_ = m_key;
    message_buff_ += sizeof(int);

    message_buff_ = m_uid.ToBuffer(message_buff_);

    return message_buff_;
}

char *MessageBase::FromBuffer(char *message_buff_)
{
    message_buff_ += sizeof(size_t);
    message_buff_ += sizeof(int);
    
    message_buff_ = m_uid.FromBuffer(message_buff_);

    return message_buff_;
}

UID MessageBase::GetUid()
{
    return m_uid;
}

size_t MessageBase::GetBufferLength() const
{
    return  m_uid.GetBufferLength();
}
