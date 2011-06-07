// #ifndef _COMPARE_PRIORITY_QUEUE_H_
// #define _COMPARE_PRIORITY_QUEUE_H_
#include "STFQNWScheduler.h"


class Compare
{
  public:
    bool operator()(buffer *a, buffer *b); 
};

// #endif