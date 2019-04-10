#include <thread>
#include <iostream>
#include <future>
#include <functional>
using namespace std;

int get_number()
{
    return 10;
}

future<int> get_number_async()
{
    promise<int> promise;
    auto future = promise.get_future();

    auto get_number_wrapper = [](std::promise<int> p) {
        p.set_value(get_number());
    };

    thread t(get_number_wrapper, move(promise));
    t.detach();
    return future;
}

future<int> schedule(function<int()> foo)
{
    promise<int> promise;
    auto future = promise.get_future();

    auto get_number_wrapper = [=](std::promise<int> p) {
        try
        {
            p.set_value(foo());
        }
        catch (...)
        {
            p.set_exception(current_exception());
        }
    };

    thread t(get_number_wrapper, move(promise));
    t.detach();
    return future;
}
