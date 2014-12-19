#include <com_example_myhand_HandUtils.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <android/log.h>

using namespace cv;

#define LOG_TAG "HandDetection/com_example_myhand_HandUtils"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

float skinRGB(Mat& image){
	if(!image.data){
		LOGD("Are you kidding me ?The tested image is null~");
		return 0.f;
	}
	float radio = 0.f;

	Mat dst_tmp = Mat::zeros(image.size(), image.type());
	Size dsize = Size(500,500);

	static const int R = 2;
	static const int G = 1;
	static const int B = 0;
	int rows = image.rows;
	int cols = image.cols;

	int SkinPixelSum = 0;

	int r, g, b;
	uchar *prgb, *pdst;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			prgb = image.data + image.step[0] * i + image.step[1] * j;
			pdst = dst_tmp.data + dst_tmp.step[0] * i + dst_tmp.step[1] * j;
			r = *(prgb + R);
			g = *(prgb + G);
			b = *(prgb + B);
			if((r>90 && g>35 && b>16 && r-b>10 && r-g>13)||
			   (r>197 && g>207 && b>167 && abs(r-b)<=18 && r>b&& g>b)){
				memcpy(pdst,prgb,3);
				SkinPixelSum++;
			}
		}
		//resize(dst_tmp, dst_tmp,dsize);
		//dst_tmp.resize(dsize);
		dst_tmp.resize(300);
		radio = SkinPixelSum * 1.0 /(image.rows * image.cols);
		return radio;
	}
}


JNIEXPORT jfloat JNICALL Java_com_example_myhand_HandUtils_nativeSkinTest
  (JNIEnv * jenv, jclass, jlong imageRgb){
	float radio = 0.f;
	try{
		radio = skinRGB( (*(Mat*)imageRgb));
		return radio;
	} catch(cv::Exception e){
		LOGD("skinRgb Function catched cv::Exception: %s", e.what());
		jclass je = jenv->FindClass("org/opencv/core/CvException");
		if(!je)
			je = jenv->FindClass("java/lang/Exception");
		jenv->ThrowNew(je, e.what());
    }catch (...){
		LOGD("skinRgb Function catched unknown exception");
		jclass je = jenv->FindClass("java/lang/Exception");
		jenv->ThrowNew(je, "Unknown exception in JNI code {highgui::VideoCapture_n_1VideoCapture__()}");
    }

}
