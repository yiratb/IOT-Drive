#ifndef ILRD_RD1556_DISPATCHER_CALLBACK_HPP
#define ILRD_RD1556_DISPATCHER_CALLBACK_HPP

#include <list> //std::list
#include <algorithm> // std::for_each

/*
- To utilize the dispatcher, observers need to inherit from the IObserver class
    and must implement the necessary methods.
- Place any necessary observer cleanup code in the DeathExec method.
- The SetDispatcher method is not relevant in this context.
- EVENTDATA is always passed by value from the publisher. 
    Keep this in mind when selecting the template type.
*/

template <typename EVENTDATA>
class IObserver;

template <typename EVENTDATA>
class Dispatcher
{
public:
    explicit Dispatcher() = default;
    ~Dispatcher();
    Dispatcher(const Dispatcher& other_) = delete;
    Dispatcher& operator=(const Dispatcher& other_) = delete;
    void Subscribe(IObserver<EVENTDATA> *observer_);
    void Unsubscribe(IObserver<EVENTDATA> *observer_);
    void Broadcast(EVENTDATA data_);

private:
    std::list<IObserver<EVENTDATA> *> m_subscribers;
};

template <typename EVENTDATA>
class IObserver
{
public:
    IObserver()  = default;
    virtual ~IObserver() = 0;
    void SetDispatcher(Dispatcher<EVENTDATA> *disp_);
    void Death();
    virtual void Notify(EVENTDATA data_) = 0;

private:
    virtual void DeathExec() = 0;
    Dispatcher<EVENTDATA> *m_dispatcher;
};

/*************************Imp Dispatcher****************************/

template <typename EVENTDATA>
inline Dispatcher<EVENTDATA>::~Dispatcher()
{
    std::for_each(m_subscribers.begin(), m_subscribers.end(), 
            [](IObserver<EVENTDATA> *observer) { observer->Death(); });
}

template <typename EVENTDATA>
inline void Dispatcher<EVENTDATA>::Subscribe(IObserver<EVENTDATA> *observer_)
{
    m_subscribers.push_back(observer_);
    observer_->SetDispatcher(this);
}

template <typename EVENTDATA>
inline void Dispatcher<EVENTDATA>::Unsubscribe(IObserver<EVENTDATA> *observer_)
{
    m_subscribers.remove(observer_);
}

template <typename EVENTDATA>
inline void Dispatcher<EVENTDATA>::Broadcast(EVENTDATA data_)
{
    std::for_each(m_subscribers.begin(), m_subscribers.end(), 
            [data_](IObserver<EVENTDATA> *observer) { observer->Notify(data_); });
}

/*************************Imp IObserver****************************/
template <typename EVENTDATA>
inline IObserver<EVENTDATA>::~IObserver()
{
    if (m_dispatcher)
    {
        m_dispatcher->Unsubscribe(this);
        m_dispatcher = nullptr;
    }
}

template <typename EVENTDATA>
inline void IObserver<EVENTDATA>::Death()
{
    if (m_dispatcher)
    {
        m_dispatcher->Unsubscribe(this);
        m_dispatcher = nullptr;
    }
    DeathExec();
}


template <typename EVENTDATA>
inline void IObserver<EVENTDATA>::SetDispatcher(Dispatcher<EVENTDATA> *disp_)
{
    m_dispatcher = disp_;
}

#endif // ILRD_RD1556_DISPATCHER_CALLBACK_HPP