#include "Lock.h"

Lock::Lock(std::mutex *mutex):m_mutex(mutex)
{
    m_mutex->lock();
}

Lock::~Lock()
{
    m_mutex->unlock();
}
