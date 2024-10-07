#ifndef ILRD_RD1556_REQUEST_WRITE_HPP
#define ILRD_RD1556_REQUEST_WRITE_HPP

#include "message_base.hpp"
#include "uid.hpp"

class RequestWrite : public MessageBase
{
public:
    explicit RequestWrite() = default;
    explicit RequestWrite(UID uid_, MessageType key_, uint32_t len_, uint64_t from_, char *buffer_);
    ~RequestWrite();

    RequestWrite(const RequestWrite& other_) = delete;
    RequestWrite& operator=(const RequestWrite& other_) = delete;

    char *ToBuffer(char *message_buff_);
    char *FromBuffer(char *message_buff_);
    size_t GetBufferLength();
    void PrintData();
    uint32_t GetLen() const;
    uint64_t GetFrom() const;
    char *GetBuffer();

private:
    uint32_t m_len;
    uint64_t m_from;
    char *m_buffer;
};

#endif //ILRD_RD1556_REQUEST_WRITE_HPP