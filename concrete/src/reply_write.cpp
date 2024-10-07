#include "reply_write.hpp"

ReplyWrite::ReplyWrite(UID uid_, MessageType key_, MinionUtility::Status status)
            : MessageBase(uid_, key_), m_status(status)
{
}

char *ReplyWrite::ToBuffer(char *message_buff_)
{
    *(size_t*)message_buff_ = GetBufferLength();
    message_buff_ += sizeof(size_t);

    message_buff_ = MessageBase::ToBuffer(message_buff_);

    *(MinionUtility::Status*)message_buff_ = m_status;
    message_buff_ += sizeof(MinionUtility::Status);

    return message_buff_;
}

char *ReplyWrite::FromBuffer(char *message_buff_)
{
    message_buff_ = MessageBase::FromBuffer(message_buff_);

    m_status = *(MinionUtility::Status*)message_buff_;
    message_buff_ += sizeof(MinionUtility::Status);

    return message_buff_;
}

size_t ReplyWrite::GetBufferLength()
{
    return MessageBase::GetBufferLength() + sizeof(MinionUtility::Status);
}

void ReplyWrite::PrintData()
{
    std::cout << "status " << m_status  << std::endl;
}

MinionUtility::Status ReplyWrite::GetStatus() const
{
    return m_status;
}
