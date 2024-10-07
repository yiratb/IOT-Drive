#ifndef ILRD_RD1556_REPLY_READ_HPP
#define ILRD_RD1556_REPLY_READ_HPP

#include "message_base.hpp"
#include "uid.hpp"
#include "minion_utility.hpp"

class ReplyRead : public MessageBase
{
public:
    explicit ReplyRead() = default;
    explicit ReplyRead(UID uid_, MessageType key_, uint32_t len_, uint64_t from_,
                 char *buffer_, MinionUtility::Status status);
    ~ReplyRead();

    ReplyRead(const ReplyRead& other_) = delete;
    ReplyRead& operator=(const ReplyRead& other_) = delete;

    char *ToBuffer(char *message_buff_);
    char *FromBuffer(char *message_buff_);
    size_t GetBufferLength();
    void PrintData();
    MinionUtility::Status GetStatus() const;
    char *GetBuffer() const;
    uint32_t GetLen() const;

private:
    uint32_t m_len;
    uint64_t m_from;
    char *m_buffer;
    MinionUtility::Status m_status; 
};

#endif //ILRD_RD1556_REPLY_READ_HPP