#include<iostream>
#include<thread>
#include<stack>
#include<Windows.h>
#include <atomic>
#include<queue>
#include<condition_variable>
#include<mutex>
#include<vector>
using namespace std;

struct port {
	int* start, * end;
};

using TInfo = int*;
class MyQueue { 
private:
std::queue<TInfo> queue; 
HANDLE semaphore; 
public:
	MyQueue() {
		semaphore = CreateSemaphore(NULL, 1, 1, NULL);
	}
	~MyQueue() { CloseHandle(semaphore); }
	void push(TInfo elem) { 
		WaitForSingleObject(semaphore, INFINITE);
		queue.push(elem); 
		ReleaseSemaphore(semaphore, 1, NULL); 
	} 
	bool try_pop(TInfo& elem) {
		bool result = false;
		WaitForSingleObject(semaphore, INFINITE);
		if (!queue.empty()) {
			result = true;
			elem = queue.front();
			queue.pop();
		}
		ReleaseSemaphore(semaphore, 1, NULL);
		return result;
	} 
	bool empty() { 
		return queue.empty(); 
	}
};

condition_variable cv;
mutex mut;
volatile long volume_work_producer = 10;
volatile long volume_work_consumer = 10;
vector<int> ans;

const int BLOCK = 10;
const int N = 4;
int n;
atomic_flag flag = ATOMIC_FLAG_INIT;
int global_max;
volatile long global_index;
atomic<int> a;
MyQueue My_Queue;

std::atomic<int> global_count;

void print_arr(int* arr)
{
	for (int i = 0; i < n; i++)
		cout<< arr[i];
	cout << endl;
.

void task_consumer()
{
	while (_InterlockedExchangeAdd(&volume_work_consumer, -1) > 0)
	{
		unique_lock<mutex> locker(mut);
		int* arr;
		cv.wait_for(locker, chrono::seconds(5), []() {return !My_Queue.empty(); });
		if (My_Queue.try_pop(arr))
		{
			int count = 0;
			for (int i = 0; i < n; i++)
			{
				if ((i == 0 && arr[i] == arr[i + 1]) || (i == n - 1 && arr[i] == arr[i - 1]) || (i > 0 && i < n - 1 && arr[i] == arr[i - 1] && arr[i] == arr[i + 1]))
				{
					count++;
				}
			}
			ans.push_back(count);
		}
		else
			_InterlockedExchangeAdd(&volume_work_consumer, 1);
	}
}

void task_produser()
{
	while (_InterlockedExchangeAdd(&volume_work_producer, -1) > 0)
	{
		srand(time(NULL));
		int* arr = new int[n];
		for (int i = 0; i < n; i++)
			/*std::cin »*/ arr[i] = rand() % 2;
		while (flag.test_and_set());
		print_arr(arr);
		flag.clear();
		this_thread::sleep_for(chrono::milliseconds(2));
		My_Queue.push(arr);
		cv.notify_all();
	}
}

int main()
{
	std::thread worker[5];
	std::cin >> n;
	srand(time(NULL));
	for (int i = 0; i < 5; i++)
		if (i < 2)
			worker[i] = thread(task_produser);
		else
			worker[i] = thread(task_consumer);
	for (int i = 0; i < 5; i++)
		worker[i].join();
	for (int i = 0; i < ans.size(); i++)
		cout << ans[i]<< ' ';
	return 0;
}
