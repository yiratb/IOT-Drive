#ifndef ILRD_RD1556_MINION_UTILITY_HPP
#define ILRD_RD1556_MINION_UTILITY_HPP

#define ALLOWED_SINGLETON
#include "singleton.hpp" //Singleton
#undef ALLOWED_SINGLETON

class MinionUtility
{
public:
    ~MinionUtility() = default;
    MinionUtility(const MinionUtility* other_) = delete;
    MinionUtility& operator=(const MinionUtility* other_) = delete;

    int GetFd() const;
    void SetFd(int fd_);
    enum CommandKey
    {
        MINION_CMD_READ,
        MINION_CMD_WRITE,
        MINION_CMD_FLUSH,
        MINION_CMD_TRIM,
        MINION_CMD_DISC
    };

    enum Status
    {
        SUCCESS,
        FAIL,
        NO_RESPONSE_YET
    };
    
private:
    explicit MinionUtility()  = default;
    friend Singleton<MinionUtility>;

    int m_fd;
};


#endif //ILRD_RD1556_MINION_UTILITY_HPP