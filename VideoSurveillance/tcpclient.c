#include "yuv422_rgb.h"
#include "tcpclient.h"


#define PORT	8888//指定通信所用端口
#define HOST_ADDR 	"192.168.0.111"//指定服务器IP地址

void tcp_init()
{
	struct sockaddr_in server;//定义服务端地址
	int pktlen,buflen;//pktien=传送数据字节数,buflen=发送缓存区长度
	char buf1[256],buf2[256];//buf1为发送缓冲,  buf2用于接收信息缓冲区
	tcp_client_fd=socket(AF_INET,SOCK_STREAM,0);//定义套接字和监听描述符
	server.sin_family=AF_INET;//指定通信协议IPV4
	server.sin_port=htons(PORT);//指定通信端口
	server.sin_addr.s_addr=inet_addr(HOST_ADDR);//设置服务器通信IP

	//connect函数,参数一为客户端socket描述符,参数二为服务端socket地址,参数三为地址长度
	if(connect(tcp_client_fd,(struct sockaddr*)&server,sizeof(server))<0)
	{
		perror("connect()");//报告连接错误信息
	}
	//close(tcp_client_fd);//关闭套接字
}

void tcp_msg_sender()
{
	int ret;
	ret = send(tcp_client_fd,  rgbdata, sizeof(rgbdata), 0);	//发送字符串
	if( ret < 0)
	{
		perror("发送失败！");
	}
}