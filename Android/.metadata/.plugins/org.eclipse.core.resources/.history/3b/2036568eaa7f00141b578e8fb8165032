#include "SkinTester.h"
/*SkinTester类提供了不同颜色空间的肤色检验方法。
 * 具体包括HSV，YCrCb,OSTU 等不同的方法；
 *
 * 最终生成二值图像，并进行形态学的相应处理
 * */

void SkinTester::skinHSV(const Mat& src, Mat& dst){
	assert(dst.channels() == 1);
	Mat HSV;
	vector<Mat> planes;
	cvtColor(src, HSV, CV_RGB2HSV);

	split(HSV, planes);
	//运用迭代器访问矩阵元素

	MatIterator_<uchar> it_H = planes[0].begin<uchar>(),
						it_H_end = planes[0].end<uchar>();
	//MatIterator_<uchar> it_Cr = planes[2].begin<uchar>();
	MatIterator_<uchar> it_skin = dst.begin<uchar>();

	//人的皮肤颜色在YCbCr色度空间的分布范围:100<=Cb<=127, 138<=Cr<=170
	for( ; it_H != it_H_end;  ++it_H, ++it_skin)
	{
		if ((7 <= *it_H) && (*it_H <= 29))
			*it_skin = 255;
		else
			*it_skin = 0;
	}
	//膨胀和腐蚀，膨胀可以填补凹洞（将裂缝桥接），腐蚀可以消除细的凸起（“斑点”噪声）
	dilate(dst, dst, Mat(5, 5, CV_8UC1), Point(-1, -1));
	erode(dst, dst, Mat(5, 5, CV_8UC1), Point(-1, -1));
};

Mat  SkinTester::skinHSV(const Mat& src){
	Mat dst(src.size(), CV_8UC1);
	skinHSV(src, dst);
	return dst;
};

void  SkinTester::skinYCC(const Mat& src, Mat& dst){
	assert(dst.channels() == 1);
	Mat YCbCr;
	vector<Mat> planes;
	//转换为YCrCb颜色空间
	cvtColor(src, YCbCr, CV_RGB2YCrCb);
	//将多通道图像分离为多个单通道图像
	split(YCbCr, planes);

	//运用迭代器访问矩阵元素

	MatIterator_<uchar> it_Cb = planes[1].begin<uchar>(),
						it_Cb_end = planes[1].end<uchar>();
	MatIterator_<uchar> it_Cr = planes[2].begin<uchar>();
	MatIterator_<uchar> it_skin = dst.begin<uchar>();

	//人的皮肤颜色在YCbCr色度空间的分布范围:100<=Cb<=127, 138<=Cr<=170
	for( ; it_Cb != it_Cb_end; ++it_Cr, ++it_Cb, ++it_skin)
	{
		if (138 <= *it_Cr &&  *it_Cr <= 170 && 100 <= *it_Cb &&  *it_Cb <= 127)
			*it_skin = 255;
		else
			*it_skin = 0;
	}

	//膨胀和腐蚀，膨胀可以填补凹洞（将裂缝桥接），腐蚀可以消除细的凸起（“斑点”噪声）
	dilate(dst, dst, Mat(5, 5, CV_8UC1), Point(-1, -1));
	erode(dst, dst, Mat(5, 5, CV_8UC1), Point(-1, -1));
};

Mat  SkinTester::skinYCC(const Mat& src){
	return src;
};

void  SkinTester::skinOSTU(const Mat& src, Mat& dst){};
Mat  SkinTester::skinOSTU(const Mat& src){
	return src;
}

void  SkinTester::skinRGB(const Mat& src, Mat& dst){

}

void  SkinTester::skinOther(const Mat& src, Mat& dst){};

void  SkinTester::skin(int TYPE, const Mat& src, Mat& dst){
	switch(TYPE){
	case HSV:
		skinHSV(src, dst);
		break;
	case YCC:
		skinYCC(src, dst);
		break;
	case OSTU:
		skinOSTU(src, dst);
		break;
	case OTHRES:
		break;
	default:
		break;
	};
}


