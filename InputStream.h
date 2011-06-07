#ifndef _INPUTSTREAM_H_
#define _INPUTSTREAM_H_

#include "NWScheduler.h"
#include "Stats.h"

class InputStream{
 public:
  /*
   * You should not need to change the public interface
   */
  InputStream(int fd, 
	      int flowId,
	      Stats *stats);
  virtual ~InputStream();
  virtual int read(char *bytes, int length);
  
 private:
  int fd;   /* File descriptor of underlying socket */
  int flowId;
  Stats *stats; 
};

#endif 
