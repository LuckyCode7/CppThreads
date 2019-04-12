#include "feastingPhilosophers.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    const int num_of_philosophs = atoi(argv[1]);

    vector<Philosoph> philosophs(num_of_philosophs);
    vector<Fork> forks(num_of_philosophs);

    setPhilosophsAndForks(philosophs,forks);
    feed_philosoph(philosophs,forks);
    showPhilosophs(philosophs);

    return 0;
}