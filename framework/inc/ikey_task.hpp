#ifndef ILRD_RD1556_IKEYTASK_HPP
#define ILRD_RD1556_IKEYTASK_HPP

class IKeyTask // for factory keys
{
public:
    virtual int GetKey() = 0;
    virtual ~IKeyTask() = default;
};

#endif //ILRD_RD1556_IKEYTASK_HPP