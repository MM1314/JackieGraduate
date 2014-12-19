#include "com_iron_grayprocess_ImageProc.h"
#include "HandReco/SkinTester.h"
#include "HandReco/FingerTest.h"
#include "HandReco/myLog.h"
#include <jni.h>
#include <android/log.h>
#include <opencv2/core/core.hpp>
#include <string>
#include <vector>

using namespace cv;
using namespace std;
int getOutput(Mat& img, int output[]);
/*
 * Class:     com_iron_grayprocess_ImageProc
 * Method:    grayProc
 * Signature: ([III)[I
 */
JNIEXPORT jintArray JNICALL Java_com_iron_grayprocess_ImageProc_grayProc
  (JNIEnv *env, jclass obj, jintArray buf, jint w, jint h){
	jint *cbuf;
	cbuf = env->GetIntArrayElements(buf, 0);
	if(cbuf == NULL){
		return 0;
	}
	//��jintArray����ת����Mat����,��Mat�Ĳ�����ʵ�Ƕ�cbuf�Ĳ���
	Mat imgData(h, w, CV_8UC4, (unsigned char *)cbuf);
	uchar* ptr = imgData.ptr(0);
	for(int i = 0; i < w * h; i++){
		//���㹫ʽ��Y(����) = 0.299*R + 0.587*G + 0.114*B
		//����һ��int���ֽڣ����ɫֵ�洢��ʽΪ��BGRA
		int grayScale = (int)(ptr[4*i+2]*0.299 + ptr[4*i+1]*0.587 + ptr[4*i+0]*0.114);
		ptr[4*i+1] = grayScale;
		ptr[4*i+2] = grayScale;
		ptr[4*i+0] = grayScale;
	}

	int size = w * h;
	jintArray result = env->NewIntArray(size);
	env->SetIntArrayRegion(result, 0, size, cbuf);
	env->ReleaseIntArrayElements(buf, cbuf, 0);
	return result;
}


JNIEXPORT jintArray JNICALL Java_com_iron_grayprocess_ImageProc_procPic
  (JNIEnv *env, jclass obj, jintArray buf, jint w, jint h){
	jint *cbuf;
	cbuf = env->GetIntArrayElements(buf, 0);
	if(cbuf == NULL){
		return 0;
	}
	Mat imgData(h, w, CV_8UC4, (unsigned char *)cbuf);
	LOGD("image data get success~~");
//	Mat dst;
//	dst.create(imgData.rows, imgData.cols, CV_8UC1);
//
//	cvtColor(imgData, dst, CV_BGRA2GRAY);
//	LOGE("before convert, the imgData's Channel() is %d", imgData.channels());
//
//	cvtColor(imgData, imgData, CV_BGRA2RGB);
//
//	cvtColor(imgData, imgData, CV_RGB2BGRA);


	//getOutput(imgData, outImage);
	/**/
	FingerTest finTester;

	Mat show_img = finTester.findContour(imgData);
	LOGE("the imgData's Channel() is %d", show_img.channels());

	if(w == show_img.cols)
		LOGD(" w = show_img.cols ");
	else
		LOGD("w != show_img.cols");

	int size = w * h;
	int* outImage = new int[size];

	getOutput(show_img, outImage);
	jintArray result = env->NewIntArray(size);
	env->SetIntArrayRegion(result, 0, size, outImage);
	env->ReleaseIntArrayElements(buf, cbuf, 0);
	return result;
}

/*�������൱�ڴ�����*/
int getOutput(Mat& img, int output[]){
	int nChannel =  img.channels();
	int nr = img.rows;
	int nc = img.cols * nChannel;
	uchar* ptr = img.ptr(0);
	switch(nChannel){
	//��ͨ��ת��Ϊ�Ҷ�ͼ����B,G,R ���أ�byte���ͣ�ȫ����ֵΪdata[i]
	case 1:
		for(int j = 0; j < nr; j++){
			uchar* data = img.ptr<uchar>(j);
			for(int i = 0; i < nc;i++){
				int tmp = j * nc + i;
				int num = data[i];
				output[tmp] = (num << 16) + (num << 8) + num;
			}
		}
		break;
	//Mat����ͨ��Ĭ��˳��ΪRGB (ע��ͨ����˳��ת�ˣ�BGR),����ת��ΪBGRA
	case 3:
		//BGR
		for(int i = 0; i < img.rows * img.cols; i++){
			//B
			int tmp = ptr[4 * i + 0];
			output[i] = (tmp & 0xFF);
			//G
			tmp = ptr[4 * i + 1];
			output[i] |= ((tmp << 8) & 0xFF00) ;
			//R
			tmp = ptr[4 * i + 2];
			output[i] |= ((tmp << 16) & 0xFF0000);
		}

//		for(int i = 0; i < img.rows * img.cols; i++){
//			//R
//			int tmp = ptr[4 * i + 0];
//			output[i] = (tmp << 16);
//			//G
//			tmp = ptr[4 * i + 1];
//			output[i] += (tmp << 8);
//			//B
//			tmp = ptr[4 * i + 2];
//			output[i] += tmp;
//		}
		break;
	//��ͨ����Ĭ��BGRA����ʹ���ǣ����ڵ��ú���ǰ����ת������
	case 4:
		for(int i = 0; i < img.rows * img.cols; i++){
			//���㹫ʽ��Y(����) = 0.299*R + 0.587*G + 0.114*B
			//����һ��int���ֽڣ����ɫֵ�洢��ʽΪ��BGRA
			int grayScale = (int)(ptr[4*i+2]*0.299 + ptr[4*i+1]*0.587 + ptr[4*i+0]*0.114);
			int tmp ;
			output[i] = ptr[4 * i + 0];
			tmp = ptr[4 * i + 1];
			output[i] += (tmp << 8);
			tmp = ptr[4 * i + 2];
			output[i] += (tmp << 16);
			tmp = ptr[4 * i + 3];
			output[i] += (tmp << 24);
		}

		/*for(int j = 0; j < nr; j++){
			uchar* data = img.ptr<uchar>(j);
			for(int i = 0; i < nc;i += 4){
				int tmp = j * nc + i;
				//B
				int num = data[i];
				output[tmp] = num;
				//G
				num = data[i + 1];
				output[tmp] += (num << 8);
				//R
				num = data[i + 2];
				output[tmp] += (num << 16);
				//A
				num = data[i + 3];
				output[tmp] += (num << 24);
			}
		}*/
		break;
	default:
		LOGE("The channel of the image is illegal~~");
		return -1;
	}
	return 0;
}
