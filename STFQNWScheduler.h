#ifndef _STFQ_NW_SCHEDULER_H_
#define _STFQ_NW_SCHEDULER_H_
#include "NWScheduler.h"
#include "sthread.h"
#include "Stats.h"
#include <queue>

typedef struct
{
  long long startTag;
  long long finishTag;
} buffer;

class STFQNWScheduler:public NWScheduler{

  public:
	STFQNWScheduler(long bytesPerSec);
	void waitMyTurn(int flowId, float weight, int lenToSend);
	long long signalNextDeadline(long long deadlineMS);
  private:
    smutex_t lock;
    scond_t okToSend;
    long maxRate;
    long long virtualTime;
    long long maxFinishTag; // maximum finish tag of any buffer that has been sent 
    long long prevFinishTag[Stats::MAX_FLOW_ID + 1]; // to access finish tag of (i-1)th of stream "flowId"
    long long nextDeadline;
    int waitingBuffers;
    std::priority_queue<buffer> pQueue;
};

#endif 
