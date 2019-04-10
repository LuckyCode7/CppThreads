#include <thread>
#include <mutex>
using namespace std;

class X
{
    mutable mutex mtx_;
    int value_ = 0;

  public:
    explicit X(int v) : value_(v) {}
    bool operator<(const X &other) const
    {
        //lock_guard<mutex> ownGuard(mtx_);
        //lock_guard<mutex> otherGuard(other.mtx_);
        scoped_lock bothLock(mtx_, other.mtx_);
        return value_ < other.value_;
    }
};