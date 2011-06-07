#include <assert.h>
#include <stdlib.h>
#include "common.h"
#include "OutputStream.h"
#include "Stats.h"

/*
 *------------------------------------------------------------------
 *
 * OutputStream --
 *
 *          Constructor.
 *
 * Arguments:
 *      int fd -- file descriptor of open network socket
 *      int flowId -- flowId (for stats)
 *      Stats *stats -- stats object shared by all output streams
 *
 * Results:
 *      Allocate a OutputStream object.
 *
 * Side effects:
 *      None.
 *
 *------------------------------------------------------------------
 */
OutputStream::OutputStream(int fd_,
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
 * ~OutputStream --
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
OutputStream::~OutputStream()
{
}


/*
 *------------------------------------------------------------------
 *
 * write --
 *
 *          Write the specified bytes to the socket.
 *
 * Arguments:
 *      char *bytes -- the buffer to send
 *      int length -- the number of bytes in the buffer
 *      int async -- boolean -- should the send be asynchronous?
 *
 * Results:
 *      Return the number of bytes sent or -1 on error.
 *
 * Side effects:
 *      None.
 *
 *------------------------------------------------------------------
 */
int 
OutputStream::write(char *bytes, int length)
{
  assert(length >= 0);
  assert(bytes != NULL);

  int got = correct_write(fd, bytes, length);
  if(got >= 0){
    stats->update(flowId, got);
  }
  return got;
}
