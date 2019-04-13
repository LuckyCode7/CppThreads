#include <vector>
#include <thread>
#include <iostream>
#include <sstream>
#include <random>
#include <mutex>
#include <sstream>
#include <chrono>

using namespace std;

class Prize
{
    string winnerId{"unknown"};
    std::random_device dev{};
    std::mt19937 rng{dev()};
    std::uniform_int_distribution<int> dist{10, 50};
    once_flag flag;

  public:
    void setWinner()
    {
        auto id = this_thread::get_id();
        auto sleepDuration = dist(rng);
        stringstream msg;
        msg << "Called " << __FUNCTION__ << "(" << id << "). Chasing time: " << sleepDuration << "ms\n";
        cout << msg.str(); // single operation on stream is atomic
        this_thread::sleep_for(chrono::milliseconds(sleepDuration));
        // TODO: set me as a winner, but don't let others overwrite this!

        call_once(flag, [&] {
            stringstream ss;
            ss << id;
            winnerId = ss.str();
        });
    }

    void showWinner()
    {
        cout << ("And the winner is... " + winnerId + '\n');
        // this is single stream operation, which is atomic - no need for mutex
    }
};

class X
{
    vector<double> values;
    once_flag flag;

    void initializeOne()
    {
        cout << __FUNCTION__ << '\n';
        call_once(flag, [&] {
            cout << "initializeOne" << endl;
            values = {1.0};
        });
    }
    void initializeTwo()
    {
        cout << __FUNCTION__ << '\n';
        call_once(flag, [&] {
            cout << "initializeTwo" << endl;
            values = {1.0, 2.0};
        });
    }
    void initializeThree()
    {
        cout << __FUNCTION__ << '\n';
        call_once(flag, [&] {
            cout << "initializeThree" << endl;
            values = {1.0, 2.0, 3.0};
        });
    }
    void initializePierdyliard() //call_once jednak nie obługuje wyjątków
    {
        cout << __FUNCTION__ << '\n';
        try{
        call_once(flag, [&] {
            cout << "Call once initializePierdyliard\n";
            throw std::bad_alloc{};
        });}
        catch(...){}
    }

  public:
    explicit X(int i) noexcept
    {
        switch (i)
        {
        case 2: // top priority
            initializeTwo();
            [[fallthrough]];
        case 3:
            initializeThree();
            [[fallthrough]];
        case std::numeric_limits<int>::max():
            initializePierdyliard();
            [[fallthrough]];
        default: // least priority
            initializeOne();
            // yes, replacing [[fallthrough]] with break is a better solution,
            // but this code is for educational purpose only ;)
        }
    }

    void print()
    {
        for (const auto &value : values)
            cout << value << " ";
        cout << "\n\n";
    }
};