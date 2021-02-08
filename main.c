#include "udpclient.h"
#include "yuv422_rgb.h"

/*****upd*****/
int client_fd;
struct sockaddr_in ser_addr;
/*************/

/*****v4l2*****/
int v4l2_fd;
//定义一个空间解码后的RGB数组
unsigned char rgbdata[camera_w*camera_h*3];
/*************/

int main(int argc, char* argv[])
{
    /*****upd初始化*****/
    udp_init(&ser_addr);

    /*****v4l2初始化*****/    
    v4l2_init();
    video_capture();  
    udp_msg_sender((struct sockaddr*)&ser_addr);    //发送udp信号

    
    close(client_fd);//关闭udp客户端


}