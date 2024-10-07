/*
    Priority Queue Header file
    Developer: Yirat
    Team: RD155-6
    Reviewer:
    Date: 19.6.24
    Status: (WIP)
*/
#ifndef ILRD_RD1556_PRIORITY_QUEUE_HPP
#define ILRD_RD1556_PRIORITY_QUEUE_HPP

#include <queue>
#include <vector>

template <typename T, typename Container = std::vector<T>, typename Compare = std::less<T> >
class PriorityQueue : private std::priority_queue<T, Container, Compare>
{
public:
    using std::priority_queue<T, Container, Compare>::pop;
    using std::priority_queue<T, Container, Compare>::push;
    using std::priority_queue<T, Container, Compare>::empty;
    using std::priority_queue<T, Container, Compare>::size;

    const T& front() const
    {
        return this->top();
    }

};

#endif // ILRD_RD1556_PRIORITY_QUEUE_HPP
