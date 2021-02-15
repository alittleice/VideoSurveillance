#include "udpclient.h"
#include "yuv422_rgb.h"

#define SERVER_PORT 8888
#define BUFF_LEN 1024                                     //一次发送多长字节
#define buf_len camera_w*camera_h*3      //总共发送多长字节//921600 
#define SERVER_IP "192.168.0.111"


void udp_msg_sender(struct sockaddr* dst)
{
    socklen_t len;
    struct sockaddr_in src;
    len = sizeof(*dst);

    //static unsigned int buf_len = sizeof(rgbdata);
    static unsigned int send_count = buf_len/BUFF_LEN; //发送次数
    static unsigned int last_len = buf_len%BUFF_LEN;
    char buf[BUFF_LEN];  //缓冲发送buf
    
    //char p[BUFF_LEN+3];                         //缓冲buf加序号之后保存的发送数据首地址
    //memset(p,0,BUFF_LEN+3);             //对发送缓冲区进行初始化，为了序号和数据拼接

    // inttostr(p,1);
    // strcat(p,"hello!");
    // printf("p:%s\n",p);
    // memcpy(buf, p, 3);
    //sendto(client_fd, "hello", 5, 0, dst, len);
    //endto(client_fd, rgbdata, buf_len, 0, dst, len);
    //printf("send_count:%d\n", send_count);

    int temp=0;
    for(temp = 0; temp < send_count; temp++)
    {
        memcpy(buf,rgbdata+temp*BUFF_LEN,BUFF_LEN);     //截取出一段发送的数据
        //inttostr(p,temp);                                                                         //标上发送的序号
        //strcat(p,buf);
        //sendto(client_fd,p, BUFF_LEN+3, 0, dst, len);
        sendto(client_fd,buf, BUFF_LEN, 0, dst, len);
        //memset(p, 0, BUFF_LEN+3);
	//sleep(1);  //一秒发送一次消息
    }
    if(last_len != 0)           //如果有零头，就发送
    {
        memcpy(buf,rgbdata+temp*BUFF_LEN,last_len);     //发送最后的零头
        sendto(client_fd, buf, last_len, 0, dst, len);
    }
}

/*
    client:
            socket-->sendto-->revcfrom-->close
*/
int udp_init(struct sockaddr_in* ser_addr)
{
    // int client_fd;
    // struct sockaddr_in ser_addr;

    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_fd < 0)
    {
        printf("create socket fail!\n");
        return -1;
    }    
    
    memset(ser_addr, 0, sizeof(*ser_addr));
    ser_addr->sin_family = AF_INET;
    ser_addr->sin_addr.s_addr = inet_addr(SERVER_IP);// 跟其他主机通信
    //ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);  //注意网络序转换 自己主机跟自己通信
    ser_addr->sin_port = htons(SERVER_PORT);  //注意网络序转换

    //udp_msg_sender((struct sockaddr*)&ser_addr);

    //close(client_fd);

    //return 0;
}

void inttostr(char *p, int num)     //int型转换3位字符串
{
    char i;

    i=num/100+48;
    *p=i;
    i=(num/10)%10+48;
    *(p+1)=i;
    i=num%10+48;
    *(p+2)=i;
}
