#include <iostream>

#include <pthread.h>
#include "thread.h"
#include "mutex.h"

mutex mymutex;

void func(void *arg)
{
  long t = reinterpret_cast<long>(arg);
  
  for(int i=0; i < 1000; ++i)
  {
    while(!mymutex.try_lock());
    std::cout << "Hello from " << t << "! ";
    mymutex.unlock();
  }
}

int main()
{
  thread t(func, reinterpret_cast<void*>(40));
  
  if(t.joinable()) t.join();
  //t.detach();

  return 0;
}

