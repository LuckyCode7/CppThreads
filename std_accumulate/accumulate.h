#pragma once
#include <cmath>
#include <thread>
#include <vector>
#include <numeric>

namespace my_accumulate
{
    template<class InputIt, class T>
    T accumulate(InputIt first, InputIt last, T init)
    {
        const unsigned int elements_counter = std::distance(first, last);
        if (!elements_counter) return init;
        const unsigned int threads_counter = std::thread::hardware_concurrency() != 0 ? std::thread::hardware_concurrency() : 2;
        const unsigned int numbersToAdd = floor(static_cast<double>(elements_counter) / static_cast<double>(threads_counter));
        std::vector<std::thread> threads;
        std::vector<T> sums(threads_counter);
    
        
        auto rememberLast = last;
        last = first;

        for (int i = 0; i < threads_counter; ++i)
        {
            if (i == threads_counter - 1)
                last = rememberLast;
            else if (last < rememberLast)
                last += numbersToAdd;

            threads.emplace_back([&]
            {
               const T partialSum = std::accumulate(first, last, 0);
               sums[i] = partialSum;
            });

            first += numbersToAdd;
        }

        for (auto& x : threads)
            x.join();

        return std::accumulate(sums.begin(), sums.end(), init);
    }
}