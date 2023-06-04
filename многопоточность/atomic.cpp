#include <iostream>
#include <vector> 
#include <atomic>
#include <thread>
int main() {
    std::vector<int> arr = { 
        2, 2, 2,2, 
        4, 5, 3,6, 
        6, 6 ,9,1, 
        1, 1, 1
    };

    std::atomic<int> count{ 0 }; 
    
    std::vector<std::thread> threads; 

    auto check_neighbors = [&](int start, int end) {
        int loccount = 0;
        for (int i = start; i < end; ++i) {
            if ( (i-1>=start)&& (i+1<=end-1)&& (arr[i] == arr[i + 1] && arr[i] == arr[i - 1])) { 
                loccount++;
                std::cout << i<<" ";
            }
        }
        std::atomic_fetch_add(&count, loccount);
    };

    int num_threads = std::thread::hardware_concurrency(); 

    int elems_per_thread = arr.size() % num_threads == 0 ? arr.size() / num_threads: (arr.size()+ (arr.size() % num_threads)) / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        int start = i * elems_per_thread;
        int end = (i == num_threads - 1) ? arr.size() : (i + 1) * elems_per_thread;
        /*if ((start == 0 && arr[start] == arr[start + 1]) ||
            start - 1 >= 0 && arr[start - 1] == arr[start] && arr[start] == arr[start + 1]) {
            std::atomic_fetch_add(&count, 1); 
            std::cout << start << " ";
        }
        if ((end == arr.size()  && arr[end - 2] == arr[end-1]) ||
            (end + 1 < num_threads) && arr[end - 1] == arr[end] && arr[end] == arr[end + 1]) {
            std::atomic_fetch_add(&count, 1);
            std::cout << end<<" ";*/
        

        threads.emplace_back(check_neighbors, start, end); 
    }

    for (auto& t : threads) {
        t.join();
    }
    int loccount = 0;
    for (int i = 0; i < num_threads; ++i) {
        int start = i * elems_per_thread;
        
        int end = (i == num_threads - 1) ? arr.size() : (i + 1) * elems_per_thread;
        if ((start == 0 && arr[start] == arr[start + 1]) ||
            start - 1 >= 0 && arr[start - 1] == arr[start] && arr[start] == arr[start + 1]) {
            loccount++;
            std::cout << start << " ";
        }
        if ((end == arr.size() && arr[end - 2] == arr[end - 1]) ||
            (end + 1 < num_threads) && arr[end - 1] == arr[end] && arr[end] == arr[end + 1]) {
            loccount++;
            std::cout << end << " ";
        }
    }
    std::atomic_fetch_add(&count, loccount);
    std::cout << "Number of numbers with equal neighbors: " << count << std::endl;

    return 0;
}
