#include "ComparePriorityQueue.h"
#include <stdio.h>

// a and b are start tags 
bool
Compare::operator()(buffer *a, buffer *b)
{
  //want lowest start tag to be at back of list, largest at front
  //so, say a < b if start tag of a is LARGER. 
  return (a->startTime > b->startTime);
}

// int main()
// {
//   Compare C;
//   bool a = true;
//   bool b = false;
//   a = C.compare(a,b);
// }