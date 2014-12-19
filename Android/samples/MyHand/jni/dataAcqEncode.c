#include <jni.h>

//#include <android/log.h>
#include <android/bitmap.h>

#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>          /* for videodev2.h */

#include <linux/videodev2.h>
#include <linux/usbdevice_fs.h>

#define  LOG_TAG    "TEST"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


#define CLEAR(x) memset (&(x), 0, sizeof (x))

#define IMG_WIDTH 640
#define IMG_HEIGHT 480

#define ERROR_LOCAL -1
#define SUCCESS_LOCAL 0

struct buffer {
        void *                  start;
        size_t                  length;
};

static char            dev_name[16];
static int              fd              = -1;
struct buffer *         buffers         = NULL;
static unsigned int     n_buffers       = 0;
static uint64_t now_pts=0;

int camerabase = -1;

int *rgb = NULL;
int *ybuf = NULL;

int yuv_tbl_ready=0;
int y1192_tbl[256];
int v1634_tbl[256];
int v833_tbl[256];
int u400_tbl[256];
int u2066_tbl[256];

unsigned char yuv422[1000000]={0};
unsigned char yuv420[1000000]={0};

int errnoexit(const char *s);

int xioctl(int fd, int request, void *arg);

int checkCamerabase(void);
int opendevice(int videoid);
int initdevice(void);
int initmmap(void);
int startcapturing(void);

int readframeonce(void);
int readframe(void);
void processimage (const void *p);

int stopcapturing(void);
int uninitdevice(void);
int closedevice(void);

void yuyv422toABGRY(unsigned char *src);
int yuv422_2_yuv420(unsigned char* yuv420, unsigned char* yuv422, int width, int height);


jint Java_com_yxp_berryhome_CameraPreview_prepareCamera( JNIEnv* env,jobject thiz, jint videoid);
jint Java_com_yxp_berryhome_CameraPreview_prepareCameraWithBase( JNIEnv* env,jobject thiz, jint videoid, jint videobase);
void Java_com_yxp_berryhome_CameraPreview_processCamera( JNIEnv* env,jobject thiz);
void Java_com_yxp_berryhome_CameraPreview_stopCamera(JNIEnv* env,jobject thiz);
void Java_com_yxp_berryhome_CameraPreview_pixeltobmp( JNIEnv* env,jobject thiz,jobject bitmap);


//01-24 07:43:06.439: E/TEST(3756): VIDIOC_DQBUF error 22, Invalid argument

/*
 VIDIOC_REQBUFS：分配内存
 VIDIOC_QUERYBUF：把VIDIOC_REQBUFS中分配的数据缓存转换成物理地址
 VIDIOC_QUERYCAP：查询驱动功能
 VIDIOC_ENUM_FMT：获取当前驱动支持的视频格式
 VIDIOC_S_FMT：设置当前驱动的频捕获格式
 VIDIOC_G_FMT：读取当前驱动的频捕获格式
 VIDIOC_TRY_FMT：验证当前驱动的显示格式
 VIDIOC_CROPCAP：查询驱动的修剪能力
 VIDIOC_S_CROP：设置视频信号的边框
 VIDIOC_G_CROP：读取视频信号的边框
 VIDIOC_QBUF：把数据放回缓存队列
 VIDIOC_DQBUF：把数据从缓存中读取出来
 VIDIOC_STREAMON：开始视频显示函数
 VIDIOC_STREAMOFF：结束视频显示函数
 VIDIOC_QUERYSTD：检查当前视频设备支持的标准，例如PAL或NTSC
 */

int errnoexit(const char *s) {
	LOGE("%s error %d, %s", s, errno, strerror (errno));
	return ERROR_LOCAL;


}

//ioctrl(fd,request,arg)
//控制IO设备，获取设备的信息和向设备发送控制参数
/*
 * fd就是用户程序打开设备时使用open函数返回的文件标示符
 * request具体的命令标志符
 */
int xioctl(int fd, int request, void *arg) {
	int r;

	do
		r = ioctl(fd, request, arg);
	while (-1 == r && EINTR == errno);

	return r;
}

//检查video设备名称
int checkCamerabase(void) {
	struct stat st;
	int i;
	int start_from_4 = 1;

	/* if /dev/video[0-3] exist, camerabase=4, otherwise, camrerabase = 0 */
	for (i = 0; i < 4; i++) {
		sprintf(dev_name, "/dev/video%d", i);
		if (-1 == stat(dev_name, &st)) {
			start_from_4 &= 0;
		} else {
			start_from_4 &= 1;
		}
	}

	LOGI("checkCamera finished");
	if (start_from_4) {
		return 4;
	} else {
		return 0;
	}
}
//打开video设备
int opendevice(int i) {
	struct stat st;

	sprintf(dev_name, "/dev/video%d", i);
	//stat() 获得文件属性，并判断是否为字符设备文件
	if (-1 == stat(dev_name, &st)) {
		LOGE("Cannot identify '%s': %d, %s", dev_name, errno, strerror (errno));
		return ERROR_LOCAL;
	}

	if (!S_ISCHR(st.st_mode)) {
		LOGE("%s is no device", dev_name);
		return ERROR_LOCAL;
	}

	fd = open(dev_name, O_RDWR);

	if (-1 == fd) {
		LOGE("Cannot open '%s': %d, %s", dev_name, errno, strerror (errno));
		return ERROR_LOCAL;
	}
	LOGI("open device finished");
	return SUCCESS_LOCAL;
}
//初始化设备
int initdevice(void) {
	struct v4l2_capability cap;
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_format fmt; //帧的格式
	unsigned int min;
	//VIDIOC_QUERYCAP 命令 来获得当前设备的各个属性
	if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
		if (EINVAL == errno) {
			LOGE("%s is no V4L2 device", dev_name);
			//fprintf(stderr,"%s is no V4L2device",dev_name);
			return ERROR_LOCAL;
		} else {
			return errnoexit("VIDIOC_QUERYCAP");
		}
	}
	//V4L2_CAP_VIDEO_CAPTURE 0x00000001
	// 这个设备支持 video capture 的接口，即这个设备具备 video capture 的功能
	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		LOGE("%s is no video capture device", dev_name);
		//fprintf(stderr,"%s is no video capture device\n",dev_name);
		return ERROR_LOCAL;
	}
	//V4L2_CAP_STREAMING 0x04000000
	// 这个设备是否支持 streaming I/O 操作函数
	if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
		LOGE("%s does not support streaming i/o", dev_name);
		//fprintf(stderr,"%s does not support streaming i/o\n",dev_name);
		return ERROR_LOCAL;
	}
	//获得设备对 Image Cropping 和 Scaling 的支持
	CLEAR(cropcap);

	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
		crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		crop.c = cropcap.defrect;

		if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop)) {
			switch (errno) {
			case EINVAL:
				break;
			default:
				break;
			}
		}
	} else {
	
	}
	//设置图形格式
	CLEAR(fmt);

	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	fmt.fmt.pix.width = IMG_WIDTH; //640
	fmt.fmt.pix.height = IMG_HEIGHT; //480

	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

	//检查流权限
	//VIDIOC_S_FMT：设置当前驱动的频捕获格式
	if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
		return errnoexit("VIDIOC_S_FMT");

	min = fmt.fmt.pix.width * 2;
	//每行像素所占的 byte 数
	if (fmt.fmt.pix.bytesperline < min)
		fmt.fmt.pix.bytesperline = min;
	min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
	if (fmt.fmt.pix.sizeimage < min)
		fmt.fmt.pix.sizeimage = min;

	LOGI("init device finished");
	return initmmap();

}
//I/O模式选择
int initmmap(void) {
	struct v4l2_requestbuffers req;

	CLEAR(req);

	req.count = 4;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	//VIDIOC_REQBUFS,分配内存
	if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			LOGE("%s does not support memory mapping", dev_name);
			//fprintf(stderr,"%s does not support memory mapping",dev_name);
			return ERROR_LOCAL;
		} else {
			return errnoexit("VIDIOC_REQBUFS");
		}
	}

	if (req.count < 2) {
		LOGE("Insufficient buffer memory on %s", dev_name);
		//fprintf(stderr,"Insufficient buffer memory on %s", dev_name);
		return ERROR_LOCAL;
	}

	buffers = calloc(req.count, sizeof(*buffers));

	if (!buffers) {
		LOGE("Out of memory");
		//fprintf(stderr,"Out of memory\n");
		return ERROR_LOCAL;
	}

	for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
		struct v4l2_buffer buf;

		CLEAR(buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n_buffers;

		if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
			return errnoexit("VIDIOC_QUERYBUF");

		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start = mmap(NULL, buf.length,
				PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);

		if (MAP_FAILED == buffers[n_buffers].start)
			return errnoexit("mmap");
	}

	return SUCCESS_LOCAL;
}

int yuv422_2_yuv420(unsigned char* yuv420, unsigned char* yuv422, int width, int height)
{
        int imgSize = width * height *2;
        int widthStep422 = width * 2;

        unsigned char* p422 = yuv422;
        unsigned char* p420y = yuv420;
        unsigned char* p420u = yuv420 + imgSize / 2;
        unsigned char* p420v = p420u + imgSize / 8;
        int i,j;
        for(i = 0; i < height; i += 2)
        {
                p422 = yuv422 + i * widthStep422;
                for( j = 0; j < widthStep422; j+=4)
                {
                        *(p420y++) = p422[j];
                        *(p420u++) = p422[j+1];
                        *(p420y++) = p422[j+2];
                }
                p422 += widthStep422;
                for( j = 0; j < widthStep422; j+=4)
                {
                        *(p420y++) = p422[j];
                        *(p420v++) = p422[j+3];
                        *(p420y++) = p422[j+2];
                }

        }
        return 0;
}

int startcapturing(void) {
	unsigned int i;
	enum v4l2_buf_type type;

	for (i = 0; i < n_buffers; ++i) {
		struct v4l2_buffer buf; //驱动中的一帧缓存

		CLEAR(buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		// VIDIOC_QBUF：把数据放回缓存队列
		if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
			return errnoexit("VIDIOC_QBUF");
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	//VIDIOC_STREAMON：开始视频显示函数
	if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
		return errnoexit("VIDIOC_STREAMON");

	LOGI("start capturing prepared");

	return SUCCESS_LOCAL;
}


int readframeonce(void) {

	for (;;) {
		fd_set fds;
		struct timeval tv;
		int r;

		FD_ZERO(&fds);
		FD_SET(fd, &fds);

		tv.tv_sec = 2;
		tv.tv_usec = 0;

		r = select(fd + 1, &fds, NULL, NULL, &tv);

		if (-1 == r) {
			if (EINTR == errno)
				continue;

			return errnoexit("select");
		}

		if (0 == r) {
			LOGE("select timeout");
			//fprintf(stderr,"select timeout\n");
			return ERROR_LOCAL;

		}

		if (readframe() == 1)
			break;
//		if(readframe() == 1)
//			printf("capture %d frame success\n",iFrameNum);

	}

	return SUCCESS_LOCAL;

}


void processimage(const void *p) {
	yuyv422toABGRY((unsigned char *) p);
}


int readframe(void) {

	struct v4l2_buffer buf;
	unsigned int i;

	CLEAR(buf);

	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	// VIDIOC_DQBUF：把数据从缓存中读取出来
	if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
		switch (errno) {
		case EAGAIN:
			return 0;
		case EIO:
		default:
			return errnoexit("VIDIOC_DQBUF");
		}
	}

	assert(buf.index < n_buffers);
	
//------------------------------------------------------
	strncpy(yuv422,buffers[buf.index].start,buffers[buf.index].length);
	yuv422_2_yuv420(yuv420,yuv422,640,480);
	video_encode_one_frame(pCodecCtxEnc,pFormatCtxEnc,pFrameEnc,yuv420,640,480);
//------------------------------------------------------

	processimage(buffers[buf.index].start);

	//VIDIOC_QBUF：把数据放回缓存队列
	if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
		return errnoexit("VIDIOC_QBUF");

	return 1;
}

int stopcapturing(void) {

	enum v4l2_buf_type type;

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
		return errnoexit("VIDIOC_STREAMOFF");

	return SUCCESS_LOCAL;

}

int uninitdevice(void) {
	unsigned int i;

	for (i = 0; i < n_buffers; ++i)
		if (-1 == munmap(buffers[i].start, buffers[i].length))
			return errnoexit("munmap");

	free(buffers);

	return SUCCESS_LOCAL;
}
//关闭设备
int closedevice(void) {
	if (-1 == close(fd)) {
		fd = -1;
		return errnoexit("close");
	}

	fd = -1;
	return SUCCESS_LOCAL;
}

void yuyv422toABGRY(unsigned char *src) {

	int width = 0;
	int height = 0;

	width = IMG_WIDTH;
	height = IMG_HEIGHT;

	int frameSize = width * height * 2;

	int i;

	if ((!rgb || !ybuf)) {
		return;
	}
	int *lrgb = NULL;
	int *lybuf = NULL;

	lrgb = &rgb[0];
	lybuf = &ybuf[0];

	if (yuv_tbl_ready == 0) {
		for (i = 0; i < 256; i++) {
			y1192_tbl[i] = 1192 * (i - 16);
			if (y1192_tbl[i] < 0) {
				y1192_tbl[i] = 0;
			}

			v1634_tbl[i] = 1634 * (i - 128);
			v833_tbl[i] = 833 * (i - 128);
			u400_tbl[i] = 400 * (i - 128);
			u2066_tbl[i] = 2066 * (i - 128);
		}
		yuv_tbl_ready = 1;
	}

	for (i = 0; i < frameSize; i += 4) {
		unsigned char y1, y2, u, v;
		y1 = src[i];
		u = src[i + 1];
		y2 = src[i + 2];
		v = src[i + 3];

		int y1192_1 = y1192_tbl[y1];
		int r1 = (y1192_1 + v1634_tbl[v]) >> 10;
		int g1 = (y1192_1 - v833_tbl[v] - u400_tbl[u]) >> 10;
		int b1 = (y1192_1 + u2066_tbl[u]) >> 10;

		int y1192_2 = y1192_tbl[y2];
		int r2 = (y1192_2 + v1634_tbl[v]) >> 10;
		int g2 = (y1192_2 - v833_tbl[v] - u400_tbl[u]) >> 10;
		int b2 = (y1192_2 + u2066_tbl[u]) >> 10;

		r1 = r1 > 255 ? 255 : r1 < 0 ? 0 : r1;
		g1 = g1 > 255 ? 255 : g1 < 0 ? 0 : g1;
		b1 = b1 > 255 ? 255 : b1 < 0 ? 0 : b1;
		r2 = r2 > 255 ? 255 : r2 < 0 ? 0 : r2;
		g2 = g2 > 255 ? 255 : g2 < 0 ? 0 : g2;
		b2 = b2 > 255 ? 255 : b2 < 0 ? 0 : b2;

		*lrgb++ = 0xff000000 | b1 << 16 | g1 << 8 | r1;
		*lrgb++ = 0xff000000 | b2 << 16 | g2 << 8 | r2;

		if (lybuf != NULL) {
			*lybuf++ = y1;
			*lybuf++ = y2;
		}
	}

}
/*
void main(){

	int i,ret;
	i = checkCamerabase();
	ret = opendevice(i);
	if(ret)
		printf("open camera success\n");
	else
		printf("open camera failure\n");

	initdevice();
	ret = startcapturing();
	if(ret)
		printf("start capture success\n");
	else
		printf("start capture failure\n");
	readframeonce();

	stopcapturing();
	uninitdevice();
	closedevice();		
}

*/

void Java_com_yxp_berryhome_CameraPreview_pixeltobmp(JNIEnv* env,
		jobject thiz, jobject bitmap) {

	jboolean bo;

	AndroidBitmapInfo info;
	void* pixels;
	int ret;
	int i;
	int *colors;

	int width = 0;
	int height = 0;

	if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
		LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
		return;
	}

	width = info.width;
	height = info.height;

	if (!rgb || !ybuf)
		return;

	if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
		LOGE("Bitmap format is not RGBA_8888 !");
		return;
	}

	if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
		LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	colors = (int*) pixels;
	int *lrgb = NULL;
	lrgb = &rgb[0];

	for (i = 0; i < width * height; i++) {
		*colors++ = *lrgb++;
	}

	AndroidBitmap_unlockPixels(env, bitmap);

}

jint Java_com_yxp_berryhome_CameraPreview_prepareCamera(JNIEnv* env,
		jobject thiz, jint videoid) {

	int ret;

	if (camerabase < 0) {
		camerabase = checkCamerabase();
	}

	ret = opendevice(camerabase + videoid);

	if (ret != ERROR_LOCAL) {
		ret = initdevice();
	}
	if (ret != ERROR_LOCAL) {
		ret = startcapturing();

		if (ret != SUCCESS_LOCAL) {
			stopcapturing();
			uninitdevice();
			closedevice();
			LOGE("device resetted");
		}

	}

	if (ret != ERROR_LOCAL) {
		rgb = (int *) malloc(sizeof(int) * (IMG_WIDTH * IMG_HEIGHT));
		ybuf = (int *) malloc(sizeof(int) * (IMG_WIDTH * IMG_HEIGHT));
		LOGI("rgb and ybuf alloc success");
	}
	return ret;
}

jint Java_com_yxp_berryhome_CameraPreview_prepareCameraWithBase(
		JNIEnv* env, jobject thiz, jint videoid, jint videobase) {

	int ret;

	camerabase = videobase;
	return Java_com_yxp_berryhome_CameraPreview_prepareCamera(env, thiz,
			videoid);

}

void Java_com_yxp_berryhome_CameraPreview_processCamera(JNIEnv* env,
		jobject thiz) {

	readframeonce();
}

void Java_com_yxp_berryhome_CameraPreview_stopCamera(JNIEnv* env,
		jobject thiz) {

	stopcapturing();

	uninitdevice();

	closedevice();

	if (rgb)
		free(rgb);
	if (ybuf)
		free(ybuf);

	fd = -1;
}

