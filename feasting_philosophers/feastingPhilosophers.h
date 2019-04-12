#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <future>
#include <iostream>
#include <random>
#include <atomic>
using namespace std;

struct Fork
{
    unsigned int ID;
    mutex fork_mutex;
};

struct Philosoph
{
    unsigned int ID;
    Fork first_fork;
    Fork second_fork;

    void eat(Fork &f1, Fork &f2)
    {
        scoped_lock lock(f1.fork_mutex, f2.fork_mutex);
        first_fork.ID = f1.ID;
        second_fork.ID = f2.ID;
    }

    friend ostream &operator<<(ostream &os, const Philosoph &p)
    {
        os << "Philosoph [" << p.ID << "]\n";
        os << "\tFork[" << p.first_fork.ID << "]\n";
        os << "\tFork[" << p.second_fork.ID << "]";
        return os;
    }
};

void setPhilosophsAndForks(vector<Philosoph> &philosophs, vector<Fork> &forks)
{
    for (unsigned int i = 0; i < philosophs.size(); ++i)
    {
        philosophs[i].ID = i;
        forks[i].ID = i;
    }
}

void showPhilosophs(vector<Philosoph> &philosophs)
{
    for (const auto &x : philosophs)
    {
        cout << x << endl;
    }
}

void feed_philosoph(vector<Philosoph> &philosophs, vector<Fork> &forks)
{
    vector<thread> threads;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> num(1, 2);

    auto setter = [&](int randomFork, int i) {
        switch (randomFork)
        {
        case 1:
        {
            if (i == 0)
                philosophs[i].eat(ref(forks[i]), ref(forks[forks.size() - 1]));
            else
                philosophs[i].eat(ref(forks[i]), ref(forks[i - 1]));
        }
        break;
        case 2:
        {
            if (i == 0)
                philosophs[i].eat(ref(forks[forks.size() - 1]), ref(forks[i]));
            else
                philosophs[i].eat(ref(forks[i - 1]), ref(forks[i]));
        }
        break;
        }
    };

    for (int i = 0; i < philosophs.size(); ++i)
    {
        threads.emplace_back(setter, num(rng), i);
    }

    for (auto &t : threads)
    {
        t.join();
    }
}