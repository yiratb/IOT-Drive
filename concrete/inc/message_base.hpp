#ifndef ILRD_RD1556_IMESSAGE_HPP
#define ILRD_RD1556_IMESSAGE_HPP

#include "uid.hpp"

class MessageBase
{
public:
    enum MessageType
    {
        REQUEST_WRITE,
        REQUEST_READ,
        REPLY_WRITE,
        REPLY_READ
    };
    explicit MessageBase() = default;
    explicit MessageBase(UID uid_, MessageType key_);
    virtual ~MessageBase() = default;

    virtual char *ToBuffer(char *message_buff_);
    virtual char *FromBuffer(char *message_buff_);
    virtual size_t GetBufferLength() const;
    UID GetUid();

protected:
    MessageType m_key;
    UID m_uid;
};

#endif //ILRD_RD1556_IMESSAGE_HPP