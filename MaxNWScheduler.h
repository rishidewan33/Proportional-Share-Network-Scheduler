#ifndef _MAX_NW_SCHEDULER_H_
#define _MAX_NW_SCHEDULER_H_
#include "NWScheduler.h"
#include "sthread.h"
class MaxNWScheduler:public NWScheduler{

  public:
	MaxNWScheduler(long bytesPerSec);
	void waitMyTurn(int flowId, float weight, int lenToSend);
	long long signalNextDeadline(long long deadlineMS);
  private:
	smutex_t lock;
	scond_t okToSend;
	scond_t alarmSignal;
	long maxRate;
	long long nextDeadline;
	bool deadlineReached;
	bool deadlineCalculated;
};
#endif 
