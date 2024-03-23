#include "mutex.h"
#include <cerrno>

mutex::mutex()
  : m(PTHREAD_MUTEX_INITIALIZER)
{
}

void mutex::lock()
{
  if(pthread_mutex_lock(&m)) throw 2;
}

void mutex::unlock()
{
  if(pthread_mutex_unlock(&m)) throw 3;
}

bool mutex::try_lock()
{
  if(pthread_mutex_trylock(&m))
  {
    if(errno == EBUSY) return false;
    else throw 4;
  }
  return true;
}
