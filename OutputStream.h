#ifndef _OUTPUTSTREAM_H_
#define _OUTPUTSTREAM_H_

#include "NWScheduler.h"
#include "Stats.h"

class OutputStream{
 public:
  /*
   * You should not need to change the public interface
   */
  OutputStream(int fd, 
	       int flowId,
	       Stats *stats);
  virtual ~OutputStream();
  virtual int write(char *bytes, int length);
  
 private:
  int fd;   /* File descriptor of underlying socket */
  Stats *stats; 
 protected:
  int flowId;
};

#endif 
