#include <vector>
#include <atomic>
#include <mutex>
#include <thread>
#include <iostream>
using namespace std;

vector<int> generateContainer()
{
    mutex m;
    vector<int> input = {2, 4, 6, 8, 10, 1, 3, 5, 7, 9};
    vector<int> output;
    vector<thread> threads;

    auto foo = [&](int i) {
        lock_guard<mutex> lock(m);
         output.push_back(input[i]); };

    for (auto i = 0u; i < input.size(); i++)
    {
        threads.emplace_back(foo, i);
    }
    for (auto &&t : threads)
    {
        t.join();
    }
    return output;
}

vector<int> generateOtherContainer()
{
    mutex m;
    int start = 5;
    atomic<bool> add = true;
    vector<int> output;
    vector<thread> threads;

    auto foo = [&](int i) {
            if (add)
            {
                lock_guard<mutex> lock(m);
                output.push_back(start += i);
            }
            else
            {
                lock_guard<mutex> lock(m);
                output.push_back(start -= i);
            }
            add = !add; };

    for (int i = 0; i < 10; i++)
    {
        threads.emplace_back(foo, i);
    }
    for (auto &&t : threads)
    {
        t.join();
    }
    return output;
}

void powerContainer(vector<int> &input)
{
    vector<thread> threads;
    auto foo = [&](int i) { input[i] *= input[i]; };

    for (auto i = 0u; i < input.size(); i++)
    {
        threads.emplace_back(foo, i);
    }
    for (auto &&t : threads)
    {
        t.join();
    }
}

void printContainer(const vector<int> &c)
{
    for (const auto &value : c)
        cout << value << " ";
    cout << endl;
}