/**
 * sender.c
 *
 * The current version sends at full speed to a receiver.
 * In the future, this program should be modified
 * to fork off a bunch of threads, each of which will
 * send using ScheduledOutputStream or using 
 * BufSchedOutputStream at specified rates.
 */
#include <assert.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include "BufSchedOutputStream.h"
#include "sthread.h"
#include "common.h"
#include "OutputStream.h"
#include "ScheduledOutputStream.h"
#include "SocketLibrary/socklib.h"
#include "Stats.h"
#include "util.h"




/*
 *------------------------------------------------------------------
 *
 * main --
 *
 *          Send data in specified rates to specified dest.
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
int main(int argc, char **argv) {
  int  socket_talk;
  Stats *stats = new Stats();
  OutputStream *os = NULL;
  int ii, ij;
  int nflows;

  if (argc < 4) {
    fprintf(stderr,
	    "(SENDER): Invoke as  'sender machine.name.address socknum nstreams [totalMax] [weight0 weight1 weight2 ...]'\n");
    exit(1);
  }


  /*
   * Note: current system only handles one flow. You will
   * want to change this.
   */
  nflows = atoi(argv[3]);
  assert(nflows > 0);
  assert(nflows < 200);



  for(ij = 0; ij < nflows; ij++){
    if ((socket_talk = sconnect(argv[1], argv[2])) < 0) {
      perror("(CLIENT): sconnect failed. Exiting.");
      exit(1);
    }

    os = new OutputStream(socket_talk, ij, stats);
    
    /*
     * The system must work when multiple threads share
     * the same InputStream or OutputStream.
     *
     * Note memory leak. We would have to keep a reference
     * count and have the last thread do the clean up to
     * free OutputSocket at the right time.
     */
    for(ii = 0; ii < ij + 1; ii++){
      sthread_t *thread = (sthread_t *)malloc(sizeof(sthread_t));
      sthread_create(thread, (void*(*)(void*))blast, (void *)os);
    }
  }
  
  printStats(stats);
}



