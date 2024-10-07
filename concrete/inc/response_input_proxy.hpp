#ifndef ILRD_RD1556_RESPONSE_INPUT_HPP
#define ILRD_RD1556_RESPONSE_INPUT_HPP

#include <mutex> //mutex

#include "iinput_proxy.hpp"
#include "nbd.hpp"

class ResponseInputProxy : public IInputProxy
{
public:
    explicit ResponseInputProxy(int fd_);
    ~ResponseInputProxy() = default;
    std::shared_ptr<IKeyTask> GetTask();
private:
    int m_fd;
};

#endif //ILRD_RD1556_RESPONSE_INPUT_HPP