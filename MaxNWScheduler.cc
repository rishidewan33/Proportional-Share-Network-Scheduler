#include <assert.h>
#include <sys/time.h>
#include "MaxNWScheduler.h"
#include "NWScheduler.h"
#include "util.h"

MaxNWScheduler::MaxNWScheduler(long bytesPerSec)
{
  //store current maximum bytes sent
  maxRate = bytesPerSec;
  nextDeadline = 0;
  deadlineReached = false;
  deadlineCalculated = true;
  
  //create lock and two condition variables
  smutex_init(&lock);
  scond_init(&okToSend);
  scond_init(&alarmSignal);
}

//-------------------------------------------------
// waitMyTurn -- return only after caller may safely
// send. If prev send s0 at time t0 transmitted b0
// bytes, the next send may not send before
// t1 >= t0 + b0/bytesPerSec
//
// NOTE: See the assignent for important restriction.
// In particular, this call must use scond_wait()
// and it may not call sthread_valesleep().
// Instead, you must rely on an alarmThread to 
// signal/broadcast when it is OK to proceed.
//
// Note: You can get the current time using
// gettimeofday(), which is defined in <sys/time.h>.
// You will need to convert the struct timeval
// into milliseconds.
//-------------------------------------------------
void MaxNWScheduler::waitMyTurn(int ignoredFlowID, float ignoredWeight, int lenToSend)
{
  /*acquire the lock */
  smutex_lock(&lock);
  /*
  While deadlineReached = true, the network is busy sending 
  a flow's data, so the current flow must wait to send
  */
  while(!deadlineReached){
	scond_wait(&okToSend,&lock);
  }
  /*
  calculate the nextDeadline, which is the amount of time
  the current flow has to send its bytes
  */
  nextDeadline = nowMS() + (1000*lenToSend/maxRate);
  deadlineCalculated = true;
  /*
  the current flow has not yet reached its deadline, 
  which we just created
  */
  deadlineReached = false;
  /*signal the alarmThread (which has been waiting in signalNextDeadline(...)) */
  scond_signal(&alarmSignal, &lock);
  /* relase the lock */
  smutex_unlock(&lock);
}


//-------------------------------------------------
// This method is called by the alarm thread.
// It
//   (1) Updates the scheduler's local state to indicate
//       that the time deadlineMS (a time expressed in
//       milliseconds) has been reached.)
//   (2) Signal/broadcast to allow any threads waiting
//       for that deadline to proceed.
//   (3) Wait until the next deadline has been calculated
//   (4) Return the next deadline to the caller
//
// Note: You can get the current time using
// gettimeofday(), which is defined in <sys/time.h>.
// You will need to convert the struct timeval
// into milliseconds.
//-------------------------------------------------
long long MaxNWScheduler::signalNextDeadline(long long prevDeadlineMS)
{
  smutex_lock(&lock);
  /*
  Entering this function means the flow's deadline has been reached
  (the alarmThread called signalNextDeadline)
  */
  deadlineReached = true;
  /*
  Time for a new flow to send, so indicate that nextDeadline has not
  been calculated, and signal for another flow (that has been waiting
  in waitMyTurn(...)) to send 
  */
  deadlineCalculated = false;
  scond_signal(&okToSend, &lock);
  //Make the alarmThread wait for the flow to calculate its deadline
  while(!deadlineCalculated){
    scond_wait(&alarmSignal, &lock);
  }
  //Right when it has calculated its deadline, it hasn't reached it yet
  deadlineReached = false;
  smutex_unlock(&lock);
  return nextDeadline;
}
