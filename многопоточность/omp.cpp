#include <iostream>
#include <vector>
#include <omp.h>

int main() {
    std::vector<int> arr = {
        2, 2, 2,2,
        4, 5, 3,6,
        6, 6 ,9,1,
        1, 1
    };

    std::atomic<int> count{ 0 };
    omp_set_num_threads(4);
#pragma omp parallel
    {
        int loccount = 0;
#pragma omp for 
        for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
            if ((i == 0 && arr[i] == arr[i + 1]) ||
                (i == static_cast<int>(arr.size()) - 1 && arr[i] == arr[i - 1]) ||
                (i > 0 && i < static_cast<int>(arr.size()) - 1 &&
                    arr[i] == arr[i - 1] && arr[i] == arr[i + 1])) {
                loccount++;
            }
            std::cout << "Thread " << omp_get_thread_num() << std::endl;
        }
#pragma omp critical
        std::atomic_fetch_add(&count, loccount);
        
    }

    std::cout << "Number of numbers with equal neighbors: " << count << std::endl;

    return 0;
}
