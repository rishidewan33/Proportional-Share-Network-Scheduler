#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "Stats.h"

/*
 *------------------------------------------------------------------
 *
 * Stats --
 *
 *          Constructor
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
 *
 *------------------------------------------------------------------
 */
Stats::Stats()
{
  smutex_init(&mutex);
  //Initialize maxFlowID to zero since we haven't seen any flows yet
  maxFlowID = 0; 
    //
    // Initialize per-object state
    //
    //           REMEMBER
    // 
    // You *must* follow the coding standards
    // distributed on the class web page.
    // Solutions failing to conform to these
    // standards will receive little or
    // no credit.
    //

}



/*
 *------------------------------------------------------------------
 *readyCondition
 * ~Stats --
 *
 *          Destructor
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
Stats::~Stats()
{
  smutex_destroy(&mutex);
}


/*
 *------------------------------------------------------------------
 *
 * update --
 *
 *          Update the count for a stream.
 *
 * Arguments:
 *      int flowId -- the Id of the stream
 *      int count -- the number of bytes sent/received
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
Stats::update(int flowId, int byteCount)
{
  //Acquire the lock.
  smutex_lock(&mutex);
  
  //Set the maxFlowID if the current flowId is larger.
  if (flowId > maxFlowID){
    maxFlowID = flowId;
  }
  
  //Indicate that you have seen the specified flowID.
  //This lets us know this flow has sent some number of bytes.
  seenFlowID[flowId] = true;
  
  //Update the number of bytes the specified flow has sent.
  totalFlowID[flowId] += byteCount;
  
  //Release the lock. 
  smutex_unlock(&mutex);
}


/*
 *------------------------------------------------------------------
 *
 * toString --
 *
 *          Return a string of counts separated by ' '.
 *
 * Arguments:
 *      char *buffer -- a buffer into which the string
 *                      should be written
 *      int maxLen -- the number of bytes available in this buffer
 *                    (Caller should always hand in large enough
 *                    buffer; callee should be sure not to write 
 *                    past the end of the buffer!)
 *
 * Results:
 *      Return the pointer to the buffer.
 *
 * Side effects:
 *      None.
 *
 *------------------------------------------------------------------
 */
char *
Stats::toString(char *buffer, int maxLen)
{
  smutex_lock(&mutex);

  int i,total,n;
  i = total = n = 0;
  //create a temporary buffer in order to store the total bytes sent by a flowID
  char *temp = (char*)malloc(maxLen);
  *buffer = '\0';
  
  /*
  Iterate through seenFlowID[]. If a flowId has sent bytes, seenFlowID[flowId]
  will be true, so add its total bytes sent to the "temp" buffer.
  Concatenate the temp buffer to "buffer", which will contain the bytes sent
  for all flowIds (and will be the final buffer returned).
  Also, keep track of total bytes sent over all flowIds with "total".
  */
  while(seenFlowID[i])
  {
    total += totalFlowID[i];;
    n = sprintf(temp,"%d ",totalFlowID[i]);
    assert(n >= 0);
    strcat(buffer,temp);
	assert(strlen(buffer) <= (unsigned int)maxLen);
    i++;
  }
  //Concatenate the total onto the end of the buffer.
  n = sprintf(temp,"%d",total);
  assert(n >= 0);
  strcat(buffer,temp);
  free(temp);
  //Clear the totalFlowID array for the next call to toString(...)
  int j;
  for(j = 0; j < i; j++)
  {
    totalFlowID[j] = 0;
  }
  //release lock
  smutex_unlock(&mutex);
  return buffer;
}

/*
 *------------------------------------------------------------------
 *
 * unitTest --
 *
 *          Verify simple things that must be true.
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
void
Stats::unitTest()
{
  
  // ***** TAKEN FROM UTIL.CC: *****
  //
  // A 64-bit long long can always be represented
  // with fewer than 21 characters; 22 includes space
  // for separator
  //
  const int MAX_CHAR_PER_ENTRY = 22;
  int maxLen = ((MAX_FLOW_ID + 1) * MAX_CHAR_PER_ENTRY) + 1;
  char buffer[maxLen]; 
  
  //Create a Stats object, update it multiple times
  Stats *statsTest = new Stats(); 
  statsTest->update(0, 3000);
  statsTest->update(0, 4325);
  statsTest->update(1, 9292);
  statsTest->update(0, 929292);
  statsTest->update(1, 32);
  statsTest->update(2, 12345);
  statsTest->update(1, 1111);
  statsTest->update(3, 9);
  statsTest->update(4, 101010);
  statsTest->update(3, 2000);
  statsTest->update(1, 12344);
  
  //update flowIDs 5 through 15, calling update 1000 times each,
  //adding a total of 500500 (= 1000 + 999 + .... + 1) bytes to each flow
  int i,j;
  for(i = 1; i <= 1000; i++){
    for (j = 5; j < 16; j++){
      statsTest->update(j, i);
    }
  }

  //Final totals should look like: 
  // 0 = 3000+4325+929292 = 936617
  // 1 = 9292+32+1111+12344 = 22779
  // 2 = 12345
  // 3 = 9 + 2000 = 2009
  // 4 = 101010
  // 5-15 = 500500
  // TOTAL = 6580260
 
  //Expected output: 
  char *expectedOutput = (char*)"936617 22779 12345 2009 101010 500500 " 
			  "500500 500500 500500 500500 500500 500500 500500 "  
			  "500500 500500 500500 6580260";
  statsTest->toString(buffer, maxLen);
  
  //Compare expected output to the result of toString(...)
  //Will return 0 if identical
  assert(strcmp(expectedOutput, buffer) == 0);
  printf("Stats self test passes.\n");
  return;
  
}
