#ifndef YUV422_RGB_H_
#define YUV422_RGB_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <string.h>
#include <sys/mman.h>

#define camera_w  640
#define camera_h  480

void yuyv_to_rgb(unsigned char *yuyvdata, unsigned char *rgbdata, int w, int h);
int v4l2_init();
int video_capture();
void v4l2_close();

//定义一个空间解码后的RGB数组
unsigned char rgbdata[camera_w*camera_h*3];

extern int v4l2_fd;
extern unsigned char *mptr[4];//保存映射后用户空间的首地址
extern unsigned int  size[4];


#endif // !UDPCLINT_H_#define 