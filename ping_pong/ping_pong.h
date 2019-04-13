#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <condition_variable>
using namespace std;

class PingPong
{
    int repetitions_;
    condition_variable cv;
    using Lock = unique_lock<mutex>;
    mutex m_;
    atomic<bool> acces{true};
    atomic<bool> run{true};
    atomic<bool> timeOut{false};
    atomic<unsigned int> ping_pong_counter{1};

  public:
    PingPong(const int repetitions)
        : repetitions_(repetitions)
    {}

    void ping()
    {
        while(ping_pong_counter <= repetitions_ && run)
        {
            Lock l(m_);
            auto status = [&]{ return acces.load() == true; };
            cv.wait(l, status);
            if(ping_pong_counter > repetitions_) break;
            cout<<"ping ["<<ping_pong_counter<<"]\n";
            acces = false;
            cv.notify_all();
            this_thread::sleep_for(500ms);
        }
    }

    void pong()
    {
        while(ping_pong_counter <= repetitions_ && run)
        {
            Lock l(m_);
            auto status = [&]{ return acces.load() == false; };
            cv.wait(l, status);
            cout<<"pong ["<<ping_pong_counter<<"]\a\n";
            acces = true;
            ++ping_pong_counter;
            cv.notify_all();
            this_thread::sleep_for(500ms);
        }
        if(ping_pong_counter > repetitions_)
        {
            timeOut = true;
        }
    }

    void stop([[maybe_unused]] chrono::seconds timeout)
    {
        Lock l(m_);
        cv.wait_for(l,timeout,[&]{return timeOut.load() == true;});
        run = false;
    }
};