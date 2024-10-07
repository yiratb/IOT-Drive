/*
    Waitable Queue Test file
    Developer: Liran
    Team: RD155-6
    Reviewer:
    Date: 18.6.24
    Status: (WIP)
*/

#include <iostream>
#include <thread>

#include "../inc/pqueue.hpp"
#include "../inc/wqueue.hpp"


template <typename T, typename CONTAINER>
void pushElements(WQueue<T, CONTAINER> &queue, const std::vector<T> &elements)
{
    for (const T &element : elements)
    {
        queue.Push(element);
        std::cout << "Pushed: " << element << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate some work
    }
}

template <typename T, typename CONTAINER>
void popElements(WQueue<T, CONTAINER> &queue, int count)
{
    for (int i = 0; i < count; ++i)
    {
        T value;
        if (queue.Pop(value, std::chrono::milliseconds(500)))
        {
            std::cout << "Popped: " << value << std::endl;
        }
        else
        {
            std::cout << "Timeout or queue is empty!" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Simulate some work
    }
}

void Test1()
{
    WQueue<int, std::queue<int>> queue1;

    std::thread pushThread1(pushElements<int, std::queue<int>>, std::ref(queue1), std::vector<int>{5, 3, 8});
    std::thread pushThread2(pushElements<int, std::queue<int>>, std::ref(queue1), std::vector<int>{2, 7, 1});

    std::thread popThread1(popElements<int, std::queue<int>>, std::ref(queue1), 3);
    std::thread popThread2(popElements<int, std::queue<int>>, std::ref(queue1), 3);

    pushThread1.join();
    pushThread2.join();
    popThread1.join();
    popThread2.join();

    std::cout << "Finished testing with std::queue\n";
}

void Test2()
{
    WQueue<int, PriorityQueue<int>> queue2;

    std::thread pushThread3(pushElements<int, PriorityQueue<int>>, std::ref(queue2), std::vector<int>{5, 3, 8});
    std::thread pushThread4(pushElements<int, PriorityQueue<int>>, std::ref(queue2), std::vector<int>{2, 7, 1});

    std::thread popThread3(popElements<int, PriorityQueue<int>>, std::ref(queue2), 3);
    std::thread popThread4(popElements<int, PriorityQueue<int>>, std::ref(queue2), 3);

    pushThread3.join();
    pushThread4.join();
    popThread3.join();
    popThread4.join();

    std::cout << "Finished testing with PriorityQueue\n";
}

void Test3()
{
    WQueue<int, PriorityQueue<int>> queue2;

    std::cout << "Test 3: pop order\n";

    std::thread pushThread3(pushElements<int, PriorityQueue<int>>, std::ref(queue2), std::vector<int>{5, 3, 8});
    std::thread pushThread4(pushElements<int, PriorityQueue<int>>, std::ref(queue2), std::vector<int>{2, 7, 1});

    pushThread3.join();
    pushThread4.join();

    std::thread popThread3(popElements<int, PriorityQueue<int>>, std::ref(queue2), 3);
    std::thread popThread4(popElements<int, PriorityQueue<int>>, std::ref(queue2), 3);

    popThread3.join();
    popThread4.join();

    std::cout << "Finished testing with PriorityQueue\n";
}

int main()
{
    Test1();
    Test2();
    Test3();
    return 0;
}
