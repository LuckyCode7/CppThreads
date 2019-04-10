#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include <random>
using namespace std;

class Fork
{
  public:
    unsigned int ID;
    mutex fork_mutex;
};

class Philosoph
{
  public:
    unsigned int ID;
    Fork first_fork;
    Fork second_fork;
    void setForks(Fork &f1, Fork &f2)
    {
        scoped_lock lock(f1.fork_mutex, f2.fork_mutex);
        first_fork.ID = f1.ID;
        second_fork.ID = f2.ID;
        cout << "Philosoph [" << ID << "]\n";
        cout << "\tFork[" << first_fork.ID << "]\n";
        cout << "\tFork[" << second_fork.ID << "]\n";
    }
};

void solution()
{
    vector<Philosoph> Philosophs(5);
    vector<Fork> Forks(5);
    vector<thread> threads;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> num(1, 2);

    for (int i = 0; i < 5; ++i)
    {
        Philosophs[i].ID = i;
        Forks[i].ID = i;
    }

    auto feed_philosoph = [&](int i,  int randomFork) {
        switch (randomFork)
        {
        case 1:
        {
            if (i == 0)
                Philosophs[i].setForks(ref(Forks[i]), ref(Forks[Forks.size() - 1]));
            else
                Philosophs[i].setForks(ref(Forks[i]), ref(Forks[i - 1]));
        }
        break;
        case 2:
        {
            if (i == 0)
                Philosophs[i].setForks(ref(Forks[Forks.size() - 1]), ref(Forks[i]));
            else
                Philosophs[i].setForks(ref(Forks[i - 1]), ref(Forks[i]));
        }
        break;
        }
    };

    for (int i = 0; i < Philosophs.size(); ++i)
    {
        threads.emplace_back(feed_philosoph, i,num(rng));
    }

    for (auto &t : threads)
    {
        t.join();
    }
}