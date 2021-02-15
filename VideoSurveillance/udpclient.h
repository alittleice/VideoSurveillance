#ifndef UDPCLIENT_H_
#define UDPCLIENT_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

void udp_msg_sender(struct sockaddr* dst);
int udp_init(struct sockaddr_in* ser_addr);
void inttostr(char *p, int num);     //int型转换3位字符串

extern int client_fd;





#endif // !UDPCLINT_H_#define 