/**
 * sendAndRecv.cc
 *
 * Send and recv data via local sockets.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include "AlarmThread.h"
#include "common.h"
#include "Flag.h"
#include "InputStream.h"
#include "MaxNWScheduler.h"
#include "NWScheduler.h"
#include "OutputStream.h"
#include "ScheduledInputStream.h"
#include "ScheduledOutputStream.h"
#include "SocketLibrary/socklib.h"
#include "Stats.h"
#include "STFQNWScheduler.h"
#include "sthread.h"
#include "util.h"



static const int THREADS_PER_RECV_SOCKET = 3;
static const char *localhost = "127.0.0.1";

struct RecvArgs{
  int port;
  Stats *stats;
  Flag *flag;
};


void
startReceiver(struct RecvArgs *args)
{
  int socket_listen;
  int socket_recv;
  int ii;
  int flowId = 0;

  /* 
   * Set up the 'listening socket'.  This establishes a network
   * IP_address:port_number that other programs can connect with.
   */
  socket_listen = setup_listen(args->port);

  /*
   * Now it is safe to try to connect and send
   */
  if(args->flag != NULL){
    args->flag->set();
  }

  while(1){
    /* 
     * Wait on the socket for a new connection to arrive.  This
     * is done using the "accept" library call.  The return value
     * of "accept" is a file descriptor for a new data socket associated
     * with the new connection.  The 'listening socket' still exists,
     * so more connections can be made to it later.
     */

    if(DEBUGGING){
      fprintf(stderr, "# Accepting connection\n");
    }
    
    socket_recv = saccept(socket_listen); 
    if (socket_recv < 0) {
      fprintf(stderr, "An error occured in the server; a connection\n");
      fprintf(stderr, "failed because of ");
      perror("Saccept failed");
      exit(1);
    }
    InputStream *is = new InputStream(socket_recv, flowId, args->stats);
    flowId++;
    //    InputStream *is = new ScheduledInputStream(socket_recv, nflows, 
    //				       stats, 1.0, nws);

    /*
     * For some threads that read from this socket.
     * Note memory leak. We would have to keep a reference
     * count and have the last thread do the clean up to
     * free InputSocket at the right time.
     */
    for(ii = 0; ii < THREADS_PER_RECV_SOCKET; ii++){
      sthread_t *thread = (sthread_t *)malloc(sizeof(sthread_t));
      sthread_create(thread, (void*(*)(void*))recv, (void *)is);
    }
  }
}





static void
startSenders(int port, int nflows, Stats *stats, NWScheduler *scheduler)
{
  OutputStream *os;

  assert(port >= MIN_SOCKET);
  assert(port <= MAX_SOCKET);

  int ij, ii;
  int socket_talk;
  static const int BIG_ENOUGH = 64;
  char portAsString[BIG_ENOUGH];
  sprintf(portAsString, "%d", port);
  assert(strlen(portAsString) < BIG_ENOUGH/2);

  if(DEBUGGING){
    printf("# Sender connecting to port %d\n", port);
  }

  for(ij = 0; ij < nflows; ij++){
    if ((socket_talk = sconnect(localhost, portAsString)) < 0) {
      perror("(CLIENT): sconnect failed. Exiting.");
      exit(1);
    }

    if(NULL == scheduler){
      os = new OutputStream(socket_talk, ij, stats);
    }
    else{
      os = new ScheduledOutputStream(socket_talk, ij, stats, 
                                     (float) ij + 1.0, scheduler);
    }      
    
    /*
     * The system must work when multiple threads share
     * the same InputStream or OutputStream.
     *
     * Note memory leak. We would have to keep a reference
     * count and have the last thread do the clean up to
     * free OutputSocket at the right time.
     */
    int nSenders;
    if(util_bytesToBlast > 0){
      nSenders = 1; // Want them to shut down at expected times
    }
    else{
      nSenders = ij+1;
    }
    assert(nSenders >= 1);
    for(ii = 0; ii < nSenders; ii++){
      sthread_t *thread = (sthread_t *)malloc(sizeof(sthread_t));
      sthread_create(thread, (void*(*)(void*))blast, (void *)os);
    }
  }

}


//
// Pick a random port between MIN and MAX to reduce
// probability that two people running on same
// machine will conflict.
//
static int
pickPort()
{
  struct timeval tv;
  int err;
  int ret;
  err = gettimeofday(&tv, NULL);
  assert(!err);
  unsigned int seed = tv.tv_sec + tv.tv_usec;
  srandom(seed);
  long int r = random();
  r = r % (MAX_SOCKET - MIN_SOCKET);
  ret = (int)(r + MIN_SOCKET);
  assert(ret >= MIN_SOCKET);
  assert(ret <= MAX_SOCKET);
  return ret;
}


/*
 *------------------------------------------------------------------
 *
 * main --
 *
 *          Create some threads to demo/test send and receive.
 *
 *          Command line is
 *            sendAndRecv [bytesToBlast [totalMx [doSTFQ]]]
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
  Stats *sendStats = new Stats();
  Stats *recvStats = new Stats();
  Flag *flag = new Flag();
  NWScheduler *scheduler;

  if(argc == 1 || argc == 2){
    scheduler = NULL;
  }
  if(argc >= 2){
    util_bytesToBlast = atoi(argv[1]);
  }
  if(argc >= 3){
    int bw = atoi(argv[2]);
    assert(bw > 0);
    if(argc == 3){
      scheduler = new MaxNWScheduler(bw);
    }
    else if(argc == 4){
      scheduler = new STFQNWScheduler(bw);
    }
    else{
      printf("Usage: sendAndRecv [bytesToBlast [bw [doSTFQ]]]\n");
      exit(-1);
    }
    sthread_t *alarm = (sthread_t *)malloc(sizeof(sthread_t));
    sthread_create(alarm, (void*(*)(void*))startAlarmThread, scheduler);
  }


  int port = pickPort();
  if(DEBUGGING){
    printf("# Using port %d\n", port);
  }

  struct RecvArgs rargs;
  rargs.port = port;
  rargs.stats = recvStats;
  rargs.flag = flag;

  if(DEBUGGING){
    printf("# starting reciver thread\n");
  }
  sthread_t *rthread = (sthread_t *)malloc(sizeof(sthread_t));
  sthread_create(rthread, (void*(*)(void*))startReceiver, (void *)&rargs);
  
  /*
   * Wait for initialization of receiver before starting senders 
   */
  flag->wait();

  if(DEBUGGING){
    printf("# starting senders\n");
  }
  startSenders(port, 5, sendStats, scheduler);

  printf("# Test will run for about 30 seconds\n");
  printStats(recvStats);
  exit(0);
}




