#ifndef _BUF_SCHED_OUTPUT_STREAM_H_
#define _BUF_SCHED_OUTPUT_STREAM_H_

#include "OutputStream.h"
#include "ScheduledOutputStream.h"
#include "NWScheduler.h"
#include "Stats.h"

class BufSchedOutputStream : public ScheduledOutputStream{
 public:
  /*
   * You should not need to change the public interface
   */
  BufSchedOutputStream(int fd, 
		       int flowId,
		       Stats *stats,
		       float weight,
		       NWScheduler *scheduler, 
		       int bufferSize);
  virtual ~BufSchedOutputStream();
  virtual int write(char *bytes, int length);
  
 private:
  /*
   * You will need to add more member variables here.
   */
};

#endif 
