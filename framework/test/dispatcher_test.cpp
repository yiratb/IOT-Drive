#include <iostream> //cout
#include <cassert> //assert

#include "dispatcher.hpp"

class ViewWindow : public IObserver<int> 
{
public:
    ViewWindow() : m_value(0), m_notified(false) {}
    ~ViewWindow() override = default;

    void Notify(int data) override 
    {
        std::cout << "ViewWindow received data: " << data << std::endl;
        m_value = data;
        m_notified = true;
    }

    bool IsNotified() const { return m_notified; }
    void Reset() { m_notified = false; }

private:
    void DeathExec() override 
    {
        std::cout << "ViewWindow DeathExec" << std::endl;
    }

    int m_value;
    bool m_notified;
};

class DataModel : public IObserver<int> 
{
public:
    DataModel() : m_value(0), m_notified(false) {}
    ~DataModel() override = default;

    void Notify(int data) override 
    {
        std::cout << "DataModel received data: " << data << std::endl;
        m_value = data;
        m_notified = true;
    }

    bool IsNotified() const { return m_notified; }
    void Reset() { m_notified = false; }

private:
    void DeathExec() override 
    {
        std::cout << "DataModel DeathExec" << std::endl;
    }

    int m_value;
    bool m_notified;
};

class ControlWindow : public IObserver<int> 
{
public:
    ControlWindow() : m_value(0), m_notified(false) {}
    ~ControlWindow() override = default;

    void Notify(int data) override 
    {
        std::cout << "ControlWindow received data: " << data << std::endl;
        m_value = data;
        m_notified = true;
    }

    bool IsNotified() const { return m_notified; }
    void Reset() { m_notified = false; }

private:
    void DeathExec() override 
    {
        std::cout << "ControlWindow DeathExec" << std::endl;
    }

    int m_value;
    bool m_notified;
};

void TestBroadcastToOneObserver()
{
    std::cout << "TestBroadcastToOneObserver:\n";

    Dispatcher<int> dispatcher;
    auto viewWindow = new ViewWindow;
    dispatcher.Subscribe(viewWindow);

    dispatcher.Broadcast(42);

    assert(viewWindow->IsNotified() && "ViewWindow should be notified.");
    viewWindow->Reset();

    delete viewWindow;
}

void TestBroadcastToTwoObservers() 
{
    std::cout << "TestBroadcastToTwoObservers:\n";

    Dispatcher<int> dispatcher;
    auto viewWindow = new ViewWindow;
    auto dataModel = new DataModel;
    dispatcher.Subscribe(viewWindow);
    dispatcher.Subscribe(dataModel);

    dispatcher.Broadcast(42);

    assert(viewWindow->IsNotified() && "ViewWindow should be notified.");
    assert(dataModel->IsNotified() && "DataModel should be notified.");
    viewWindow->Reset();
    dataModel->Reset();

    delete viewWindow;
    delete dataModel;
}

void TestBroadcastToThreeObservers() 
{
    std::cout << "TestBroadcastToThreeObservers:\n";

    Dispatcher<int> dispatcher;
    auto viewWindow = new ViewWindow;
    auto dataModel = new DataModel;
    auto controlWindow = new ControlWindow;
    dispatcher.Subscribe(viewWindow);
    dispatcher.Subscribe(dataModel);
    dispatcher.Subscribe(controlWindow);

    dispatcher.Broadcast(42);

    assert(viewWindow->IsNotified() && "ViewWindow should be notified.");
    assert(dataModel->IsNotified() && "DataModel should be notified.");
    assert(controlWindow->IsNotified() && "ControlWindow should be notified.");
    viewWindow->Reset();
    dataModel->Reset();
    controlWindow->Reset();

    delete viewWindow;
    delete dataModel;
    delete controlWindow;
}

void TestRemoveOneObserverDuringBroadcast() 
{
    std::cout << "TestRemoveOneObserverDuringBroadcast:\n";

    Dispatcher<int> dispatcher;
    auto viewWindow = new ViewWindow;
    auto dataModel = new DataModel;
    dispatcher.Subscribe(viewWindow);
    dispatcher.Subscribe(dataModel);

    dispatcher.Unsubscribe(viewWindow);
    dispatcher.Broadcast(42);

    assert(!viewWindow->IsNotified() && "ViewWindow should not be notified.");
    assert(dataModel->IsNotified() && "DataModel should be notified.");
    dataModel->Reset();

    delete viewWindow;
    delete dataModel;
}

void TestRemoveAllObserversDuringBroadcast() 
{
    std::cout << "TestRemoveAllObserversDuringBroadcast:\n";

    Dispatcher<int> dispatcher;
    auto viewWindow = new ViewWindow;
    auto dataModel = new DataModel;
    dispatcher.Subscribe(viewWindow);
    dispatcher.Subscribe(dataModel);

    dispatcher.Unsubscribe(viewWindow);
    dispatcher.Unsubscribe(dataModel);
    dispatcher.Broadcast(42);

    assert(!viewWindow->IsNotified() && "ViewWindow should not be notified.");
    assert(!dataModel->IsNotified() && "DataModel should not be notified.");

    delete viewWindow;
    delete dataModel;
}

void TestAddObserverDuringBroadcast() 
{
    std::cout << "TestAddObserverDuringBroadcast:\n";

    Dispatcher<int> dispatcher;
    auto viewWindow = new ViewWindow;

    dispatcher.Subscribe(viewWindow);
    dispatcher.Broadcast(42);
    assert(viewWindow->IsNotified() && "ViewWindow should be notified.");
    viewWindow->Reset();

    auto dataModel = new DataModel;
    dispatcher.Subscribe(dataModel);
    dispatcher.Broadcast(84);

    assert(viewWindow->IsNotified() && "ViewWindow should be notified.");
    assert(dataModel->IsNotified() && "DataModel should be notified.");
    viewWindow->Reset();
    dataModel->Reset();

    delete viewWindow;
    delete dataModel;
}

void TestAddTwoObserversDuringBroadcast() 
{
    std::cout << "TestAddTwoObserversDuringBroadcast:\n";

    Dispatcher<int> dispatcher;
    auto viewWindow = new ViewWindow;

    dispatcher.Subscribe(viewWindow);
    dispatcher.Broadcast(42);
    assert(viewWindow->IsNotified() && "ViewWindow should be notified.");
    viewWindow->Reset();

    auto dataModel = new DataModel;
    auto controlWindow = new ControlWindow;
    dispatcher.Subscribe(dataModel);
    dispatcher.Subscribe(controlWindow);
    dispatcher.Broadcast(84);

    assert(viewWindow->IsNotified() && "ViewWindow should be notified.");
    assert(dataModel->IsNotified() && "DataModel should be notified.");
    assert(controlWindow->IsNotified() && "ControlWindow should be notified.");
    viewWindow->Reset();
    dataModel->Reset();
    controlWindow->Reset();

    delete dataModel;
    delete viewWindow;
    delete controlWindow;
}


int main()
{
    TestBroadcastToOneObserver();
    std::cout << "\n";
    TestBroadcastToTwoObservers();
    std::cout << "\n";
    TestBroadcastToThreeObservers();
    std::cout << "\n";
    TestRemoveOneObserverDuringBroadcast();
    std::cout << "\n";
    TestRemoveAllObserversDuringBroadcast();
    std::cout << "\n";
    TestAddObserverDuringBroadcast();
    std::cout << "\n";
    TestAddTwoObserversDuringBroadcast();
    std::cout << "\n";

    std::cout << "All tests passed!" << std::endl;

    return 0;
}
