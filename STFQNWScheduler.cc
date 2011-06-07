#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include "STFQNWScheduler.h"
#include "util.h"

STFQNWScheduler::STFQNWScheduler(long bytesPerSec)
{
  //set maxRate, the maximum allowable bytes to send per second
  maxRate = bytesPerSec;
  
  //initialize our variables
  virtualTime = 0;
  maxFinishTag = 0;
  nextDeadline = 0;
  waitingBuffers = 0;
  
  //create a lock and a condition variable
  smutex_init(&lock);
  scond_init(&okToSend);

}

/*
  Overloading the < operator for our priority queue so that
  it is sorted in order of descending startTags (rather, the 
  smallest startTag will be popped)
*/
bool operator<(const buffer &a, const buffer &b)
{
  return a.startTag > b.startTag;
}

void STFQNWScheduler::waitMyTurn(int flowId, float weight, int lenToSend)
{
  //acquire lock
  smutex_lock(&lock);
  
  //create a buffer for the current flow
  buffer buff;
  //set the buffer's start and finish tags according to the project description
  buff.startTag = std::max(prevFinishTag[flowId], virtualTime);
  buff.finishTag = buff.startTag + lenToSend/weight;
  //end create buffer
  
  //set the maximum finish tag we have seen
  maxFinishTag = std::max(buff.finishTag,maxFinishTag);
  
  //push the buffer onto the priority queue
  pQueue.push(buff);
  
  /*
    If there are no buffers in the queue (network is not busy) 
    and no buffers are waiting to be sent, set the virtualTime 
    to the maximum finish tag of any buffer that has been sent. 
  */
  if(pQueue.empty() && waitingBuffers == 0)
  {
	virtualTime = maxFinishTag;
  }
  
  /*
    Make the current flow wait as long as the current time has not reached
    its prescribed deadline or the startTag of the current buffer does not 
    match the startTag of the buffer on the top of the queue (which has the 
    lowest startTag of all buffers on the queue).
  */
  while(nowMS() < nextDeadline || buff.startTag != pQueue.top().startTag)
  {
	waitingBuffers++;
	scond_wait(&okToSend, &lock);
	waitingBuffers--;
  }
  
  //Set the virtual time to the currently sending buffer's startTag
  virtualTime = buff.startTag;
  
  //set the nextDeadline as prescribed in the project requirements
  nextDeadline = nowMS() + (1000*lenToSend/maxRate);
  
  //set the previous finishTag for the specified stream
  prevFinishTag[flowId] = buff.finishTag;
  
  pQueue.pop();
  
  //release the lock
  smutex_unlock(&lock);
}

long long STFQNWScheduler::signalNextDeadline(long long prevDeadline)
{
  //acquire the lock
  smutex_lock(&lock);
  //signal that it is ok for another buffer to send
  scond_signal(&okToSend,&lock);
  //release the lock
  smutex_unlock(&lock);
  return nextDeadline;
}