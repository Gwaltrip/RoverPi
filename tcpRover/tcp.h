#ifndef TCP_H
#define TCP_H
void tcpError(const char *msg);
void *tcpListener(void *arg);


int n;
int newsockfd;
int portno;
char mode;
#endif
