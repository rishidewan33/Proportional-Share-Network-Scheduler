#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SocketLibrary/socklib.h"
#include "Stats.h"
#include "sthread.h"
#include "util.h"

int util_bytesToBlast = 0;

/*
 *------------------------------------------------------------------
 *
 * printStats --
 *
 *          Every 1 second, print the stats for the active streams.
 *
 * Arguments:
 *      Stats *stats -- the stats summary object
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      None.
 *
 *------------------------------------------------------------------
 */
void
printStats(Stats *stats)
{
  int sec = 0;
  static const int dieTime = 30; // print for 30 seconds, then die

  //
  // A 64-bit long long can always be represented
  // with fewer than 21 characters; 22 includes space
  // for separator
  //
  const int MAX_CHAR_PER_ENTRY = 22;
  int maxLen = ((stats->MAX_FLOW_ID + 1) * MAX_CHAR_PER_ENTRY) + 1;
  char buffer[maxLen]; 

  while(1){
    //
    // Warning: this is one of the few times when
    // it is OK to sleep. See the programming 
    // standards document.
    //
    sec++;
    sthread_sleep(1, 0);
    stats->toString(buffer, maxLen);
    printf("%d %s\n", sec, buffer);
    fflush(stdout);
    if(sec > dieTime){
      return;
    }
  }
}


/*
 *------------------------------------------------------------------
 *
 * blast --
 *
 *          Send at full speed on an OutputSocket.
 *
 *          ***************************************
 *          *****             NOTE         ********
 *          *****    DO NOT MODIFY BLAST!  ********
 *          ***************************************
 * 
 *          Instead, extend OutputStream to do scheduling.
 *
 *          The sending code should be oblivious to the 
 *          underlying scheduling.
 *
 * Arguments:
 *      OutputStream os -- stream on which to send
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      None.
 *
 *------------------------------------------------------------------
 */
void 
blast(OutputStream *os)
{
  int got;
  long long tot = 0;
  static const int BUF_SIZE = 2048;
  char buffer[BUF_SIZE];

  if(DEBUGGING){
    printf("# Blast thread started\n");
  }
  while(1){
    got = os->write(buffer, BUF_SIZE);
    if(got <= 0){
//    if(DEBUGGING){
	printf("# Done sending after %lld bytes; send thread done.\n", tot);
//    }
      return;
    }
    tot += got;
    if(util_bytesToBlast){
      if(tot >= util_bytesToBlast){
        // We don't close the stream since it may
        // be shared by several blasters.
        // This is a resource leak.
        return;
      }
    }
  }
}


/*
 *------------------------------------------------------------------
 *
 * recv --
 *
 *          Receive fromt the InputStream at full speed.
 *
 *          Note -- for the scheduling, you should not
 *          need to modify receive, just hand in
 *          a ScheduledInputStream.
 *
 * Arguments:
 *      InputStream *is -- the input stream on which to receive.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      None.
 *
 *------------------------------------------------------------------
 */
void 
recv(InputStream *is)
  {
    int got;
    long long tot = 0;
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    if(DEBUGGING){
      printf("# Receive thread started\n");
    }
    while(1){
      got = is->read(buffer, BUFFER_SIZE);
      if(got <= 01){
	if(DEBUGGING){
	  printf("# Done receiving after %lld bytes; recv thread done\n", tot);
	}
	return;
      }
      tot += got;
    }
  }



/*
 *------------------------------------------------------------------
 *
 * setup_listen --
 *
 *    This function a port number, and opens up
 *    a listening socket on that port.  In
 *    case of error, this function simply bonks out.
 *
 * Arguments:
 *      int portNumber -- port on which to listen
 *                            representing the socket on which to 
 *                            listen.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      None.
 *
 *------------------------------------------------------------------
 */
int 
setup_listen(int port) 
{
  static const int BIG_ENOUGH = 64;

  int socket_listen;
  char asString[BIG_ENOUGH];
  
  if(port < MIN_SOCKET){
    fprintf(stderr, "(RECEIVER): port number too small");
    exit(-1);
  }

  if(port > MAX_SOCKET){
    fprintf(stderr, "(RECEIVER): port number too large");
    exit(-1);
  }

  sprintf(asString, "%d", port);
  assert(strlen(asString) < BIG_ENOUGH/2);
  
  if ((socket_listen = slisten(asString)) < 0) {
    perror("(RECEIVER): slisten");
    exit(-1);
  }

  return socket_listen;
}

static const long long MS_PER_SEC = 1000;
static const long long US_PER_MS = 1000;
long long 
timeValToMS(struct timeval *tv)
{
  long long ms = (long long)tv->tv_sec * MS_PER_SEC + (long long)tv->tv_usec / US_PER_MS;
  return ms;
}

long long 
nowMS()
{
  struct timeval tv;
  int err;
  err = gettimeofday(&tv, NULL);
  assert(!err);
  return timeValToMS(&tv);
}
