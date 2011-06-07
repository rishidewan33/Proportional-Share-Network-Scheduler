#ifndef _FLAG_H_
#define _FLAG_H_

#include "sthread.h"

class Flag{
 public:
  Flag();
  ~Flag();
  void set();
  void wait();
 private:
  smutex_t mutex;
  scond_t cond;
  int value;
};

#endif /* _BARRIER_H_ */
