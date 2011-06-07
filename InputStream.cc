#include <assert.h>
#include <stdlib.h>
#include "common.h"
#include "InputStream.h"
#include "NWScheduler.h"
#include "Stats.h"

/*
 *------------------------------------------------------------------
 *
 * InputStream --
 *
 *          Constructor.
 *
 * Arguments:
 *      int fd -- file descriptor of open network socket
 *      int flowId -- flowId (for stats)
 *      Stats *stats -- stats object shared by all input streams
 *
 * Results:
 *      Allocate a InputStream object.
 *
 * Side effects:
 *      None.
 *
 *------------------------------------------------------------------
 */
InputStream::InputStream(int fd_,
			   int flowId_,
			   Stats *stats_)
{
  assert(fd_ > 0);
  assert(flowId_ >= 0);
  assert(stats_ != NULL);


  fd = fd_;
  flowId = flowId_;
  stats = stats_;
}


/*
 *------------------------------------------------------------------
 *
 * ~InputStream --
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
InputStream::~InputStream()
{
}


/*
 *------------------------------------------------------------------
 *
 * read --
 *
 *          Read the specified bytes from the socket.
 *
 * Arguments:
 *      char *bytes -- the buffer to send
 *      int length -- the number of bytes in the buffer
 *
 * Results:
 *      Return the number of bytes sent or -1 on error.
 *      Return 0 on EOF.
 *    
 * Side effects:
 *      None.
 *
 *------------------------------------------------------------------
 */
int InputStream::read(char *bytes, int length)
{
  assert(length >= 0);
  assert(bytes != NULL);

  int got = correct_read(fd, bytes, length);
  if(got > 0){
    stats->update(flowId, got);
  }
  return got;
}
