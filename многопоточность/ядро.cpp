#include<iostream> 
#include<thread> 
#include<queue> 
#include<Windows.h> 
#include <atomic> 

struct port {
    int* start, * end;
};

using TInfo = port;

class MyQueue {
private:
    std::queue<TInfo> queue;
    HANDLE semaphore;

public:
    MyQueue() {
        semaphore = CreateSemaphore(NULL, 0, INT_MAX, NULL);
    }

    ~MyQueue() {
        CloseHandle(semaphore);
    }

    void push(TInfo elem) {
        queue.push(elem);
        ReleaseSemaphore(semaphore, 1, NULL);
        //увеличивает значение semaphore на 1, чтобы обозначить, что ресурс становится доступным. 
    }

    bool try_pop(TInfo& elem) {
        bool result = false;
        WaitForSingleObject(semaphore, INFINITE);
        //блокирует поток и ждет, пока  semaphore не будет доступен
        if (!queue.empty()) {
            result = true;
            elem = queue.front();
            queue.pop();
        }
        return result;
    }

    bool empty() {
        return queue.empty();
    }
};

const int BLOCK = 10;
const int N = 4;
const int COUNT = 16;

MyQueue myQueue;

std::atomic<int> global_count;

void Fill(int* a, int count)
{
    for (int i = 0; i < count; i++)
    {
        std::cin>>a[i];
    }
}

void task_consumer()
{
    port port;
    while (!myQueue.empty())
    {
        if (myQueue.try_pop(port))
        {
            int count = 0;
            for (int* p = port.start + 1; p < port.end; ++p)
            {
                if (*p == *(p - 1) && *p == *(p + 1))
                {
                    count++;
                }
            }
            global_count += count;
        }
    }
}

int main()
{
    std::thread worker[N];
    int arr[COUNT];
    Fill(arr, COUNT);
    if (arr[0] == arr[1]) global_count++;
    if (arr[COUNT - 1] == arr[COUNT - 2]) global_count++;
    for (int i = 0; i < COUNT / BLOCK; i++)
    {
        port port;
        port.start = &arr[i * BLOCK];
        port.end = port.start + BLOCK;
        myQueue.push(port);
    }
    for (int i = 0; i < N; i++)
    {
        worker[i] = std::thread(task_consumer);
    }
    for (int i = 0; i < N; i++)
    {
        worker[i].join();
    }
    std::cout << global_count << '\n';
    return 0;
}