/**
 * receiver.c
 *
 * The current version receives at full speed from whatever
 * senders contact it.
 * In the future, this program should be modified
 * so that each receiver socket can be throttled to
 * receive at a specified rate via ScheduledInputThread.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include "common.h"
#include "InputStream.h"
#include "ScheduledInputStream.h"
#include "SocketLibrary/socklib.h"
#include "Stats.h"
#include "sthread.h"
#include "util.h"



/*
 * The OutputStream objects should allow multiple threads
 * to send on the same OutputStream. Test this.
 */
static const int THREADS_PER_SOCKET = 3;

/*
 *------------------------------------------------------------------
 *
 * main --
 *
 *          Accept incoming connections, and fork off
 *          some threads to receive on them.
 *
 * Arguments:
 *      See usage message.
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
main(int argc, char **argv)
{
  int socket_listen;
  int socket_recv;
  int nflows;
  int ii;
  Stats *stats = new Stats();

  if (argc < 2) {
    fprintf(stderr,
	    "(RECEIVER): Invoke as  'receiver socknum [totalMax] [weight0 weight1 weight2 ...]'\n");
    exit(1);
  }

  /*
   * Note: current system only handles one flow. You will
   * want to change this.
   */
  nflows = 0;
  
  /*
   * TBD: You probably should create a synchronized object
   * that tracks the number of flows. This object will be shared
   * between this 'main' thread and a 'stats' thread
   * that you create. The 'stats' thread should periodically
   * print out the Stats.
   */
  printf("TBD: create a stats thread to print info from the Stats object.\n");

  /* 
   * Set up the 'listening socket'.  This establishes a network
   * IP_address:port_number that other programs can connect with.
   */
  socket_listen = setup_listen(atoi(argv[1]));

  //  NWScheduler *nws = new NWScheduler();


  while(1){
    /* 
     * Wait on the socket for a new connection to arrive.  This
     * is done using the "accept" library call.  The return value
     * of "accept" is a file descriptor for a new data socket associated
     * with the new connection.  The 'listening socket' still exists,
     * so more connections can be made to it later.
     */

    if(DEBUGGING){
      fprintf(stderr, "Accepting connection\n");
    }
    
    socket_recv = saccept(socket_listen); 
    if (socket_recv < 0) {
      fprintf(stderr, "An error occured in the server; a connection\n");
      fprintf(stderr, "failed because of ");
      perror("Saccept failed");
      exit(1);
    }
    nflows++;
    InputStream *is = new InputStream(socket_recv, nflows, stats);
    //    InputStream *is = new ScheduledInputStream(socket_recv, nflows, 
    //				       stats, 1.0, nws);

    /*
     * For some threads that read from this socket.
     * Note memory leak. We would have to keep a reference
     * count and have the last thread do the clean up to
     * free InputSocket at the right time.
     */
    for(ii = 0; ii < THREADS_PER_SOCKET; ii++){
      sthread_t *thread = (sthread_t *)malloc(sizeof(sthread_t));
      sthread_create(thread, (void*(*)(void*))recv, (void *)is);
    }

  }
}

