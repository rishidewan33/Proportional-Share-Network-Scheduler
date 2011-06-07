#ifndef _SCHEDULED_INPUTSTREAM_H_
#define _SCHEDULED_INPUTSTREAM_H_

#include "InputStream.h"
#include "NWScheduler.h"
#include "Stats.h"

class ScheduledInputStream : public InputStream{
 public:
  /*
   * You should not need to change the public interface
   */
  ScheduledInputStream(int fd, 
		       int flowId,
		       Stats *stats,
		       float weight,
		       NWScheduler *scheduler);
  ~ScheduledInputStream();
  virtual int read(char *bytes, int length);
  
 private:
  /* 
   * You will probably have to add some new stuff here
   */
};

#endif 
