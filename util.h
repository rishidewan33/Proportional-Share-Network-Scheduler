#ifndef _UTIL_H_
#define _UTIL_H_
#include "InputStream.h"
#include "OutputStream.h"

const int DEBUGGING = 0;
const int MIN_SOCKET = 1025;
const int MAX_SOCKET = 30000;

extern int util_bytesToBlast;

void printStats(Stats *stats);
void blast(OutputStream *os);
void recv(InputStream *os);
int setup_listen(int port);
long long timeValToMS(struct timeval *tv);
long long nowMS();
#endif
