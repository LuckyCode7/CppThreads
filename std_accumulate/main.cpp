#include <iostream>
#include <string>
#include <chrono>
#include "accumulate.h"

int main()
{
    std::vector<int> v(1000000, 7);

    auto clock_start = std::chrono::high_resolution_clock::now();    
    my_accumulate::accumulate(v.begin(), v.end(), 0);
    auto clock_end = std::chrono::high_resolution_clock::now();     

    auto time = clock_end - clock_start;
    std::cout << time.count() << std::endl;

    system("PAUSE");
    return 0;
}