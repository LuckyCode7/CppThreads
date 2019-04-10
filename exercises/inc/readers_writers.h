#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>
using namespace std;

shared_mutex shared_numbers;
mutex stream_mutex;

vector<int> numbers = {};


int getNextValue()
{
    static int i = 0;
    return i += 10;
}

void read(int index)
{
    shared_lock<shared_mutex> lock(shared_numbers);
    int value = numbers[index];
    lock.unlock();
    lock_guard<mutex> stream_lock(stream_mutex);
    cout << value << " ";
}

void write()
{
    lock_guard<shared_mutex> lock(shared_numbers);
    int newValue = getNextValue();
    numbers.emplace_back(newValue);
}
