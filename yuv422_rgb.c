//YUV转RGB程序demo
#include	"yuv422_rgb.h"

/*
 *VGA：640*480
 *
 * 
 *
 */
void yuyv_to_rgb(unsigned char *yuyvdata, unsigned char *rgbdata, int w, int h)
{
	//码流Y0 U0 Y1 V1 Y2 U2 Y3 V3 --》YUYV像素[Y0 U0 V1] [Y1 U0 V1] [Y2 U2 V3] [Y3 U2 V3]--》RGB像素
	int r1, g1, b1;
	int r2, g2, b2;
	int i = 0;
	for(i; i<w*h/2; i++)
	{
	    char data[4];
	    memcpy(data, yuyvdata+i*4, 4);
	    unsigned char Y0=data[0];
	    unsigned char U0=data[1];
	    unsigned char Y1=data[2];
	    unsigned char V1=data[3];
		//Y0U0Y1V1  -->[Y0 U0 V1] [Y1 U0 V1]
	    r1 = Y0+1.4075*(V1-128); if(r1>255)r1=255; if(r1<0)r1=0;
	    g1 =Y0- 0.3455 * (U0-128) - 0.7169*(V1-128); if(g1>255)g1=255; if(g1<0)g1=0;
	    b1 = Y0 + 1.779 * (U0-128);  if(b1>255)b1=255; if(b1<0)b1=0;

	    r2 = Y1+1.4075*(V1-128);if(r2>255)r2=255; if(r2<0)r2=0;
	    g2 = Y1- 0.3455 * (U0-128) - 0.7169*(V1-128); if(g2>255)g2=255; if(g2<0)g2=0;
	    b2 = Y1 + 1.779 * (U0-128);  if(b2>255)b2=255; if(b2<0)b2=0;

	    rgbdata[i*6+0]=r1;
	    rgbdata[i*6+1]=g1;
	    rgbdata[i*6+2]=b1;
	    rgbdata[i*6+3]=r2;
	    rgbdata[i*6+4]=g2;
	    rgbdata[i*6+5]=b2;
	}
}

unsigned char *mptr[4];//保存映射后用户空间的首地址
unsigned int  size[4];		//mapbuffer的长度
int v4l2_init()
{
//1.打开设备
	v4l2_fd = open("/dev/video0", O_RDWR);
	if(v4l2_fd < 0)
	{
		perror("error:打开设备失败");
		return -1;
	}

	//3.设置采集格式
	struct v4l2_format vfmt;
	vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;//摄像头采集
	vfmt.fmt.pix.width = camera_w;//设置宽（不能任意）
	vfmt.fmt.pix.height = camera_h;//设置高
	vfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;//设置视频采集格式
	vfmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
	
	int ret = ioctl(v4l2_fd, VIDIOC_S_FMT, &vfmt);
	if(ret < 0)
	{
		perror("error:设置格式失败");
	}

	memset(&vfmt, 0, sizeof(vfmt));
	vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret  = ioctl(v4l2_fd, VIDIOC_G_FMT, &vfmt);
	if(ret < 0)
	{
		perror("获取格式失败");
	}

	if(vfmt.fmt.pix.width == camera_w && vfmt.fmt.pix.height == camera_h);
	else
	{
		printf("格式设置失败\n");
	}
#if 0
	unsigned char *p = (unsigned char *)&vfmt.fmt.pix.pixelformat;
	if(p[0]=='Y'&&p[1]=='U'&&p[2]=='Y'&&p[3]=='V')
		printf("格式设置成功\n");
	else
		printf("格式设置失败\n");
	printf("pixelformat=%c%c%c%c\n", p[0],p[1],p[2],p[3]);
#endif

	//4.申请内核空间
	struct v4l2_requestbuffers reqbuffer;
	reqbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbuffer.count = 4; //申请4个缓冲区
	reqbuffer.memory = V4L2_MEMORY_MMAP ;//映射方式
	ret  = ioctl(v4l2_fd, VIDIOC_REQBUFS, &reqbuffer);
	if(ret < 0)
	{
		perror("error5:申请队列空间失败");
	}

	//5.映射

	struct v4l2_buffer mapbuffer;
	//初始化type, index
	mapbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	int i = 0;
	for(i =0; i<4; i++)
	{
		mapbuffer.index = i;
		ret = ioctl(v4l2_fd, VIDIOC_QUERYBUF, &mapbuffer);//从内核空间中查询一个空间做映射
		if(ret < 0)
		{
			perror("error6:查询内核空间队列失败");
		}
		mptr[i] = (unsigned char *)mmap(NULL, mapbuffer.length, PROT_READ|PROT_WRITE,MAP_SHARED, v4l2_fd, mapbuffer.m.offset);
		size[i]=mapbuffer.length;

		//通知使用完毕--‘放回去’
		ret  = ioctl(v4l2_fd, VIDIOC_QBUF, &mapbuffer);
		if(ret < 0)
		{
			perror("error7:放回失败");
		}
	}

}

struct v4l2_buffer  readbuffer;
int video_capture(void)
{
	//6.开始采集
	int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	int ret = ioctl(v4l2_fd, VIDIOC_STREAMON, &type);
	if(ret < 0)
	{
		perror("开启失败");
	}

	int count = 1;
	while(count--)      //循环采集数据
	{

		//7.从队列中提取一帧数据
		//struct v4l2_buffer  readbuffer;
		readbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		ret = ioctl(v4l2_fd, VIDIOC_DQBUF, &readbuffer);
		if(ret < 0)
		{
			perror("提取数据失败");
		}

#if 0	//保存yuv文件
		FILE *file=fopen("my.yuv", "w+");
		//mptr[readbuffer.index]
		fwrite(mptr[readbuffer.index], readbuffer.length, 1, file);
		fclose(file);
#endif
#if 0	//保存RGB文件
	FILE *file2=fopen("my.rgb", "w+");
	//mptr[readbuffer.index]
	fwrite(rgbdata, sizeof(rgbdata), 1, file2);
	fclose(file2);
#endif
		yuyv_to_rgb(mptr[readbuffer.index], rgbdata, camera_w, camera_h);
		
		//检测数据大小是否正常
		// if(readbuffer.length/2*3 == sizeof(rgbdata))
		// 	printf("rgb图像大小:%d\n", sizeof(rgbdata));
		// else
		// 	printf("图像大小错误!\n");

		//通知内核已经使用完毕
		ret = ioctl(v4l2_fd, VIDIOC_QBUF, &readbuffer);
		if(ret < 0)
		{
			perror("放回队列失败");
		}

	}
	return 0;
}

void v4l2_close()
{
	int ret;
	//8.停止采集
	ret = ioctl(v4l2_fd, VIDIOC_STREAMOFF, &readbuffer.type);
	if(ret < 0)
	{
		perror("VIDIOC_STREAMOFF 失败");
	}

	//9.释放映射
	int i;
	for( i=0; i<4; i++)
		munmap(mptr[i], size[i]);

	//10.关闭设备
	close(v4l2_fd);
}