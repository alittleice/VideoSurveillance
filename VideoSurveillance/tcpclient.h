#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

#include <stdio.h>//标准输入输出头文件
#include <stdlib.h>//标准库头文件
#include <string.h>//字符数组相关函数定义
#include <sys/socket.h>//Linux下套接字相关函数定义
#include <sys/types.h>//linux下基本数据类型头文件
#include <netinet/in.h>//通信传输协议相关函数定义
#include <unistd.h>//可移植系统接口相关定义
#include <arpa/inet.h>//提供ip地址转换函数
#include "sys/stat.h"
#include "sys/ioctl.h"
#include "fcntl.h"
#include <poll.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>

void tcp_msg_sender();
void tcp_init();

extern int tcp_client_fd;





#endif // !UDPCLINT_H_#define 