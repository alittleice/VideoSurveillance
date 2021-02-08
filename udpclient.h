#ifndef UDPCLIENT_H_
#define UDPCLIENT_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

void udp_msg_sender(struct sockaddr* dst);
int udp_init(struct sockaddr_in* ser_addr);

extern int client_fd;





#endif // !UDPCLINT_H_#define 