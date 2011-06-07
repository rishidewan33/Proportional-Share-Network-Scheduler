#ifndef _NW_SCHEDULER_H_
#define _NW_SCHEDULER_H_
class NWScheduler{
 public:

  //
  // Not all arguments used for all descendents of
  // NWScheduler.
  //
  virtual void waitMyTurn(int flowId, float weight, int lenToSend) = 0;

  //
  // An alarm thread should call scheduler when
  // deadlines arrive.
  //
  virtual long long signalNextDeadline(long long deadlineMS) = 0;
};
#endif 
