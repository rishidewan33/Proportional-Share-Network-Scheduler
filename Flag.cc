#include "Flag.h"

Flag::Flag()
{
  value = 0;
  smutex_init(&mutex);
  scond_init(&cond);
}

Flag::~Flag()
{
  smutex_destroy(&mutex);
  scond_destroy(&cond);
}

void 
Flag::set()
{
  smutex_lock(&mutex);
  value = 1;
  scond_broadcast(&cond, &mutex);
  smutex_unlock(&mutex);
}

void
Flag::wait()
{
  smutex_lock(&mutex);
  while(value == 0){
    scond_wait(&cond, &mutex);
  }
  smutex_unlock(&mutex);
}
