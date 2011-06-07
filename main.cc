/* 
   c++ -D_POSIX_PTHREAD_SEMANTICS main.cc sthread.cc -lpthread  -lrt
   or 
   g++ -Wall -D_POSIX_PTHREAD_SEMANTICS main.cc sthread.cc -lpthread -lrt
*/
#ifndef _POSIX_PTHREAD_SEMANTICS
#define _POSIX_PTHREAD_SEMANTICS
#endif

#include <assert.h>
#include <pthread.h>
#include <iostream.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "sthread.h"

void *foo(void *);
void condWaitUntilDone(void);
void bar(int id);

static const int NTHREADS = 10;

smutex_t mutex;
scond_t countCondition;
scond_t readyCondition;
int global = 0;
int ready = 0;

int
main(int argc, char **argv)
{
  pthread_t tid[NTHREADS];
  int ii;
  int args[NTHREADS];

  smutex_init(&mutex);
  scond_init(&readyCondition);
  scond_init(&countCondition);

  for(ii = 0; ii < NTHREADS; ii++){
    args[ii] = ii;
    sthread_create(&tid[ii], foo, (void *)&args[ii]);
  }

  condWaitUntilDone();

  smutex_destroy(&mutex);
  scond_destroy(&readyCondition);
  scond_destroy(&countCondition);
}


void
condWaitUntilDone(void)
{
  smutex_lock(&mutex);{
    assert(ready == 0);
    cout << "Main says go!\n";
    ready = 1;
    scond_broadcast(&readyCondition, &mutex);
    while(global < NTHREADS){
      scond_wait(&countCondition, &mutex);
      cout << "Main woke up. Global is " << global << "\n";
    }
  }smutex_unlock(&mutex);

}

void *foo(void *arg)
{
  int *i = (int *)arg;
  if(*i == 3){
    cout << "Thread " << *i << " is going to sleep.\n";

    /*
     * (ALMOST) NEVER USE SLEEP!!!!
     * Sleep is included in this library for the
     * rare instances where you need to sleep for
     * a fixed amount of time and then do something.
     * 
     * One of the most common errors in undergraduate
     * threads projects is using sleep when what
     * should be used is scond_wait().
     *
     * If you use sleep when you should use wait, we 
     * will count off enourmous numbers of points
     * on your project!!! If you don't know
     * when to use which, find out before you
     * start writing code!!!
     */
    sthread_sleep(5, 0); /* DANGER */


    cout << "Thread " << *i << " worke up.\n";
  }

  bar(*i);
  sthread_exit();
  return NULL; // NOTREACHED
}

void bar(int id){
  smutex_lock(&mutex);{
    while(!ready){
      scond_wait(&readyCondition, &mutex);
    }
    global++;
    cout << "Thread " << id << " is running. Global is " << global << "\n";
    scond_signal(&countCondition, &mutex);
  }smutex_unlock(&mutex);
  return;
}
