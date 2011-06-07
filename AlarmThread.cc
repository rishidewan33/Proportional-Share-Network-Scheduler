#include <assert.h>
#include <sys/time.h>
#include "AlarmThread.h"
#include "util.h"

static void sleepMS(long long timeToSleepMS);


void 
startAlarmThread(NWScheduler *scheduler)
{
  long long deadline = 0;
  long long now;
  while(1){
    deadline = scheduler->signalNextDeadline(deadline);
    now = nowMS();
    if(deadline > now)
    {
      //
      // This is the only place you can sleep()
      // in this project.
      //
      sleepMS(deadline - now);
    }
    now = nowMS();
    assert(now >= deadline);
  }
}


static const int MS_PER_SEC = 1000;
static const int NS_PER_MS = 1000000;
static void sleepMS(long long timeToSleepMS)
{
  unsigned int seconds = timeToSleepMS / MS_PER_SEC;
  unsigned int nanoseconds = (timeToSleepMS - seconds * 1000) * NS_PER_MS;
  sthread_sleep(seconds, nanoseconds);
}

