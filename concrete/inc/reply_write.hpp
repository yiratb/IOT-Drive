#ifndef ILRD_RD1556_REPLY_WRITE_HPP
#define ILRD_RD1556_REPLY_WRITE_HPP

#include "message_base.hpp"
#include "uid.hpp"
#include "minion_utility.hpp"

class ReplyWrite : public MessageBase
{
public:
    explicit ReplyWrite() = default;
    explicit ReplyWrite(UID uid_, MessageType key_, MinionUtility::Status status);
    ~ReplyWrite() = default;

    ReplyWrite(const ReplyWrite& other_) = delete;
    ReplyWrite& operator=(const ReplyWrite& other_) = delete;

    char *ToBuffer(char *message_buff_);
    char *FromBuffer(char *message_buff_);
    size_t GetBufferLength();
    void PrintData();
    MinionUtility::Status GetStatus() const;
private:
    MinionUtility::Status m_status;
};

#endif //ILRD_RD1556_REPLY_WRITE_HPP