#ifndef  thread_h
#define  thread_h

#include <pthread.h>

class thread
{
    bool valid;
    pthread_t t;
    void (*f)(void*);
    void *a;
    bool _joinable;
    
  public:
    thread();
    thread(void(*func)());
    thread(void(*func)(void*),void *arg);
   ~thread();
    
    bool joinable() const;
    void join();
    void detach();
};

#endif //thread_h

