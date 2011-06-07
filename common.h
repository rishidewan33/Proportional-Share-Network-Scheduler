/**
 * common.h
 *
 * Declares some functions and constants that are useful
 * to both the client and server.
 */

#ifdef __cplusplus
extern "C" {
#endif

#define REQUEST_SIZE 10
#define RESPONSE_SIZE 100

int correct_read(int s, char *data, int len);
int correct_write(int s, char *data, int len);
void  send_response(int fd, char *response, int response_length);
#ifdef __cplusplus
}
#endif
