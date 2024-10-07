#ifndef ILRD_RD1556_REQUEST_READ_HPP
#define ILRD_RD1556_REQUEST_READ_HPP

#include "message_base.hpp"
#include "uid.hpp"

class RequestRead : public MessageBase
{
public:
    explicit RequestRead() = default;
    explicit RequestRead(UID uid_, MessageType key_, uint32_t len_, uint64_t from_);
    ~RequestRead() = default;

    RequestRead(const RequestRead& other_) = delete;
    RequestRead& operator=(const RequestRead& other_) = delete;

    char *ToBuffer(char *message_buff_);
    char *FromBuffer(char *message_buff_);
    size_t GetBufferLength();
    void PrintData();
    uint32_t GetLen() const;
    uint64_t GetFrom() const;
    
private:
    uint32_t m_len;
    uint64_t m_from;
};

#endif //ILRD_RD1556_REQUEST_READ_HPP