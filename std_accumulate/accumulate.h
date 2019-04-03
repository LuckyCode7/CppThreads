#pragma once
#include <math.h>
#include <thread>

std::mutex m_;

template<class InputIt, class T>
T accumulate(InputIt first, InputIt last, T init)
{
    const unsigned int threads_counter = thread::hardware_concurrency() != 0 ? thread::hardware_concurrency() : 2;
    const unsigned int elements_counter = std::distance(first, last);

    if (!elements_counter) return init;

    bool hardWork = false; // last thread has to sum up more numbers then the rest of threads 
    std::vector<thread> sums;
    unsigned int numbersToAdd = 0;
    unsigned int numbersToAddCounter = 0;

    if (elements_counter % threads_counter == 0)
    {
        numbersToAdd = elements_counter / threads_counter;
    }
    else
    {
        numbersToAdd = floor(static_cast<double>(elements_counter) / static_cast<double>(threads_counter));
        hardWork = true;
    }

    for (int i = 0; i < threads_counter; ++i)
    {
        sums.emplace_back([&] 
        {
            for (; first != last; ++first)
            {
                if((numbersToAddCounter == numbersToAdd) && !(i = threads_counter - 1 && hardWork))
                {   
                    numbersToAddCounter = 0;
                    break;
                }
                m_.lock();
                init = std::move(init) + *first;
                m_.unlock();
                numbersToAddCounter++;
            }
        });
    }

    for (auto& x : sums)
        x.join();

    return init;
}