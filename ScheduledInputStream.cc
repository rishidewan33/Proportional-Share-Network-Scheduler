#include <assert.h>
#include <stdlib.h>
#include "common.h"
#include "InputStream.h"
#include "ScheduledInputStream.h"
#include "NWScheduler.h"
#include "Stats.h"

/*
 *------------------------------------------------------------------
 *
 * ScheduledInputStream --
 *
 *          Constructor.
 *
 * Arguments:
 *      int fd -- file descriptor of open network socket
 *      int flowId -- flowId (for stats)
 *      Stats *stats -- stats object shared by all input streams
 *      float weight -- the weight of this stream for the 
 *                      proportional share scheduler
 *      NWScheduler scheduler -- the scheduler
 *
 * Results:
 *      Allocate a ScheduledInputStream object.
 *
 * Side effects:
 *      None.
 *
 *------------------------------------------------------------------
 */
ScheduledInputStream::ScheduledInputStream(int fd_,
					   int flowId_,
					   Stats *stats_,
					   float weight_,
					   NWScheduler *scheduler_)
  : InputStream(fd_, flowId_, stats_)
{
  assert(weight_ >= 0);
  assert(scheduler_ != NULL);
}


/*
 *------------------------------------------------------------------
 *
 * ~ScheduledInputStream --
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
ScheduledInputStream::~ScheduledInputStream()
{
}


/*
 *------------------------------------------------------------------
 *
 * read --
 *
 *          Read the specified bytes from the socket
 *          when it is my turn to use the network.
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
int ScheduledInputStream::read(char *bytes, int length)
{
  /*
   * TBD: wait my turn
   */
  return InputStream::read(bytes, length);
}
