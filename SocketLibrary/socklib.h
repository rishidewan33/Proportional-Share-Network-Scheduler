/*
 * socklib.h 
 */

#ifdef __cplusplus
extern "C" {
#endif

extern int saccept (int s);
extern int sconnect (const char *hostname, const char *servicename);
extern int slisten (const char *servicename);
extern int sportnum (int s);

extern char *serror ();
extern char *sename;
extern int serrno;

extern void sclrerr (void);
extern void sperror (char *msg);

extern int  test_accept(int socket_listen, int *returnedSocket);
extern int  incoming_messages(int s);
extern int  get_next_message(int s, char c[]);
extern int  send_a_message(int s, char *c);
extern int  empty_incoming_messages(int s);
extern int  wait_for_message(int s, int time);

/* these return -1 for error, 0 for would block, 1 for would succeed */
extern int test_ready(int s);
extern int test_writey(int s);

int     make_inetaddr (char *hostname, char *servicename, 
		       struct sockaddr_in *inaddr);
int     protonumber (char *protoname);

#define SE_NOERR	(0)
#define SE_SYSERR	(1)
#define SE_UNKAF	(2)
#define SE_UNKHOST	(3)
#define SE_UNKSERV	(4)
#define SE_UNKPROT	(5)
#define SE_NONMEM       (6)

#define MAXBUFF 2056

#ifdef __cplusplus
}
#endif

