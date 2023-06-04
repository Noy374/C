#include <iostream>
#include <windows.h>
#include <vector>

using namespace std;


struct ThreadData {
    int startIndex;
    int endIndex;
    int* array;
    int* count;
    HANDLE mutex;
};


DWORD WINAPI CountNeighbors(LPVOID lpParam) {
    ThreadData* data = (ThreadData*)lpParam;
    int count = 0;

    for (int i = data->startIndex; i <= data->endIndex; i++) {
        if ((i == 0 || data->array[i - 1] == data->array[i]) &&
            (i == data->endIndex || data->array[i + 1] == data->array[i])) {
            count++;
        }
    }

    WaitForSingleObject(data->mutex, INFINITE);
    *(data->count) += count; 
    ReleaseMutex(data->mutex); 
    return 0;
}

int main() {
    const int n = 12; 
    const int blockSize = 3; 
    int count = 0; 
    int array[n];
    HANDLE threads[n / blockSize];
    vector<ThreadData> threadData(n / blockSize);
    HANDLE mutex = CreateMutex(NULL, FALSE, NULL); 
    for (int i = 0; i < n; i++) {
        cin >> array[i];
    }
    cout << "Array: ";
    for (int i = 0; i < n; i++) {
        if (i % 4 == 0) cout << endl;
        cout << array[i] << " ";
        
    }
    cout << endl;
    for (int i = 0; i < n / blockSize; i++) {
        threadData[i].startIndex = i * blockSize;
        threadData[i].endIndex = min((i + 1) * blockSize - 1, n - 1);
        threadData[i].array = array;
        threadData[i].count = &count;
        threadData[i].mutex = mutex;
        threads[i] = CreateThread(NULL, 0, CountNeighbors, &(threadData[i]), 0, NULL);
    }
    WaitForMultipleObjects(n / blockSize, threads, TRUE, INFINITE);
    cout << "Count: " << count << endl;


    return 0;
}
