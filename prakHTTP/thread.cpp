#include "thread.h"
#include <cerrno>
#include <cstdio>
#include <iostream>


thread::thread()
    : valid(false), f(0), a(0), _joinable(true)
{
}

thread::thread(void(*func)())
    : valid(false), f(reinterpret_cast<void(*)(void*)>(func)), a(0), _joinable(true)
{
    if(pthread_create(&t, 0, reinterpret_cast<void*(*)(void*)>(f), 0)) {throw 1;  perror("2");}
    else
        valid = true;
}

thread::thread(void(*func)(void*), void *arg)
    : valid(false), f(func), a(arg), _joinable(true)
{
    if(pthread_create(&t, 0, reinterpret_cast<void*(*)(void*)>(f), a)) {throw 1; perror("2");}
    else
        valid = true;
}

thread::~thread()
{
    if(_joinable && valid)
    {
        if(pthread_join(t, 0)) {throw 2; perror("2");}
        else
        {
        valid     = false;
        _joinable = false;
        }
    }
}

bool thread::joinable() const
{
  return _joinable;
}

void thread::join()
{
  if(_joinable && valid)
  {
    if(pthread_join(t, 0)) {throw 3; perror("2");}
    else
    {
        valid     = false;
        _joinable = false;
    }
  }
}

void thread::detach()
{
    if(_joinable && valid)
    {
        if(pthread_detach(t)) {throw 4; perror("2");}
        else
        {
            valid     = false;
            _joinable = false;
        }
    }
}

void thread::joinable(bool b){
    _joinable = b;
}

pthread_t thread::gett(){
    return t;
}
