#ifndef mutex_h
#define mutex_h

#include <pthread.h>

class mutex
{
    pthread_mutex_t m;
    
  public:
    mutex();
    
    void lock();
    void unlock();
    bool try_lock();
};

#endif  //mutex_h
