#ifndef _SCHED_OUTPUT_STREAM_H_
#define _SCHED_OUTPUT_STREAM_H_

#include "OutputStream.h"
#include "NWScheduler.h"
#include "Stats.h"

class ScheduledOutputStream : public OutputStream{
 public:
  /*
   * You should not need to change the public interface
   */
  ScheduledOutputStream(int fd, 
			int flowId,
			Stats *stats,
			float weight,
			NWScheduler *scheduler);
  virtual ~ScheduledOutputStream();
  virtual int write(char *bytes, int length);
  
 private:
  float weight;
  NWScheduler *scheduler;
};

#endif 
