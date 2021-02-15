#include "tcpclient.h"
#include "yuv422_rgb.h"

/*****upd*****/
 int client_fd;
 struct sockaddr_in ser_addr;
/*************/

/*****tcp*****/
int tcp_client_fd;
/*************/

/*****v4l2*****/
int v4l2_fd;
//定义一个空间解码后的RGB数组
unsigned char rgbdata[camera_w*camera_h*3];
/*************/

unsigned char recebuf[3];
int main(int argc, char* argv[])
{
    int ret;
    /*****upd初始化*****/
    udp_init(&ser_addr);

     /*****tcp初始化*****/
    //tcp_init();
    
    /*****v4l2初始化*****/    
    v4l2_init();

    int count = 1;
    while(count--)
    {
    video_capture();  

    /*****发送数据*****/    
    udp_msg_sender((struct sockaddr*)&ser_addr);    //发送udp信号
    //tcp_msg_sender();                                                                //发送tcp信号

    //read(tcp_client_fd, recebuf, sizeof(recebuf));
    //printf("recebuf:%s\n",recebuf);

    }
    
    v4l2_close();
    //close(tcp_client_fd);//关闭tcp套接字
    close(client_fd);//关闭udp客户端

}