#ifndef LANDING_LOCK_H
#define LANDING_LOCK_H


#include <mutex>

class Lock
{
public:
    Lock(std::mutex *mutex);
    ~Lock();
private:
    std::mutex *m_mutex;
};


#endif //LANDING_LOCK_H
