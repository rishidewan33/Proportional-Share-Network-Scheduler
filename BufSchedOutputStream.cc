#include <assert.h>
#include <stdlib.h>
#include "common.h"
#include "BufSchedOutputStream.h"
#include "NWScheduler.h"
#include "OutputStream.h"
#include "ScheduledOutputStream.h"
#include "Stats.h"

/*
 *------------------------------------------------------------------
 *
 * BufSchedOutputStream --
 *
 *          Constructor.
 *
 * Arguments:
 *      int fd -- file descriptor of open network socket
 *      int flowId -- flowId (for stats)
 *      Stats *stats -- stats object shared by all output streams
 *      NWScheduler *scheduler -- scheduler shared by all 
 *                                BufSchedOutputStreams
 *
 * Results:
 *      Allocate a BufSchedOutputStream object.
 *
 * Side effects:
 *      None.
 *
 *------------------------------------------------------------------
 */
BufSchedOutputStream::BufSchedOutputStream(int fd_,
					   int flowId_,
					   Stats *stats_,
					   float weight_,
					   NWScheduler *scheduler_,
					   int bufLen)
  : ScheduledOutputStream(fd_, flowId_, stats_, weight_, scheduler_)
{
  assert(bufLen >= 0);
}


/*
 *------------------------------------------------------------------
 *
 * ~BufSchedOutputStream --
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
BufSchedOutputStream::~BufSchedOutputStream()
{
}


/*
 *------------------------------------------------------------------
 *
 * write --
 *
 *          Copy the bytes into the buffer
 *          and return immediately unless the buffer is full.
 *
 *          If the buffer is full,
 *          wait until the buffer has room for all of the bytes,
 *          copy them into the buffer, and return.
 *
 *          If the data to be sent is larger than the buffer,
 *          send the data directly on the underlying ScheduledOutputStream,
 *          bypassing the buffer and blocking until the send 
 *          completes.
 *
 *          Warning: be careful how you manage the buffer. Your
 *          solution should ensure that neither starvation nor
 *          deadlock is possible.
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
int BufSchedOutputStream::write(char *bytes, int length)
{
  assert(length >= 0);
  assert(bytes != NULL);

  /*
   * TBD: implement buffering and scheduling logic
   */

  return ScheduledOutputStream::write(bytes, length);
}
