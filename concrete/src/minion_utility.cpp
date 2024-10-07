#include "minion_utility.hpp"

int MinionUtility::GetFd() const
{
    return m_fd;
}

void MinionUtility::SetFd(int fd_)
{
    m_fd = fd_;
}