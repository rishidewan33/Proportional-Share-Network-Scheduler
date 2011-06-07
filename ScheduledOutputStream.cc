#include <assert.h>
#include <stdlib.h>
#include "common.h"
#include "OutputStream.h"
#include "ScheduledOutputStream.h"
#include "Stats.h"

/*
 *------------------------------------------------------------------
 *
 * ScheduledOutputStream --
 *
 *          Constructor.
 *
 * Arguments:
 *      int fd -- file descriptor of open network socket
 *      int flowId -- flowId (for stats)
 *      Stats *stats -- stats object shared by all output streams
 *      NWScheduler *scheduler -- scheduler shared by all 
 *                                ScheduledOutputStreams
 *
 * Results:
 *      Allocate a ScheduledOutputStream object.
 *
 * Side effects:
 *      None.
 *
 *------------------------------------------------------------------
 */
ScheduledOutputStream::ScheduledOutputStream(int fd_,
					     int flowId_,
					     Stats *stats_,
					     float weight_,
					     NWScheduler *scheduler_)
  : OutputStream(fd_, flowId_, stats_)
{
  weight = weight_;
  scheduler = scheduler_;
  assert(scheduler_ != NULL);
  assert(weight_ >= 0.0);
}


/*
 *------------------------------------------------------------------
 *
 * ~ScheduledOutputStream --
 *
 *          Destructor.
 *
 * Arguments:
 *      None.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      None.
 *
 *------------------------------------------------------------------
 */
ScheduledOutputStream::~ScheduledOutputStream()
{
}


/*
 *------------------------------------------------------------------
 *
 * write --
 *
 *          Wait until it is this socket's fair turn
 *          to write, then write the specified bytes to the socket
 *          and update stats.
 *
 * Arguments:
 *      char *bytes -- the buffer to send
 *      int length -- the number of bytes in the buffer
 *
 * Results:
 *      Return the number of bytes sent or -1 on error.
 *
 * Side effects:
 *      None.
 *
 *------------------------------------------------------------------
 */
int ScheduledOutputStream::write(char *bytes, int length)
{
  assert(length >= 0);
  assert(bytes != NULL);

  /*
   * TBD: wait my turn and update stats
   * e.g. scheduler.waitMyTurn(flowId, length, ...)
   * ...
   */
  
  scheduler->waitMyTurn(flowId, weight, length); 
  
  //update occurs in OutputStream::write();
  
  return OutputStream::write(bytes, length);


}
