#include <vector>
#include <iostream>
#include <future>
#include <chrono>
#include "ping_pong.h"
using namespace std;

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        return -1;
    }
    int repetitions = stoi(argv[1]);
    int timeout = stoi(argv[2]);
    
    PingPong pp(repetitions);
    thread t1(&PingPong::ping, &pp);
    thread t2(&PingPong::pong, &pp);
    thread t3(&PingPong::stop, &pp, chrono::seconds(timeout));
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
