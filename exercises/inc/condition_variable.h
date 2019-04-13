#include <iostream>
#include <deque>
#include <thread>
#include <string>
#include <fstream>
#include <mutex>
#include <condition_variable>
using namespace std;

template <typename T>
class ThreadsafeQueue
{
    deque<T> queue_;
    mutable mutex queue_m;
    condition_variable cv;
  public:
    void push(const T &element)
    {
        unique_lock<mutex> lock(queue_m);
        queue_.push_front(element);
        cv.notify_all();
    }

    T pop()
    {
        unique_lock<mutex> lock(queue_m);
        auto hasData = [&]{ return !queue_.empty(); };
        cv.wait(lock, hasData);
        auto top = queue_.back();
        queue_.pop_back();
        return top;
    }

    bool empty() const
    {
        unique_lock<mutex> lock(queue_m);
        return queue_.empty();
    }
};

using StringQueue = ThreadsafeQueue<string>;

void provideData(StringQueue &sq)
{
    string txt;
    while (getline(cin, txt))
        sq.push(txt);
}

void saveToFile(StringQueue &sq)
{
    ofstream file("sth.txt");
    while (file)
    {
        file << sq.pop() << endl;
    }
}

void produceText(StringQueue &sq, int number)
{
    for (int i = 0; i < number; i++)
        sq.push("This is random text number " + to_string(i)); 
}