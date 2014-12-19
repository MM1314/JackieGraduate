#include "SkinTester.h"
/*SkinTester���ṩ�˲�ͬ��ɫ�ռ�ķ�ɫ���鷽����
 * �������HSV��YCrCb,OSTU �Ȳ�ͬ�ķ�����
 *
 * �������ɶ�ֵͼ�񣬲�������̬ѧ����Ӧ����
 * */

void SkinTester::skinHSV(const Mat& src, Mat& dst){
	assert(dst.channels() == 1);
	Mat HSV;
	vector<Mat> planes;
	cvtColor(src, HSV, CV_RGB2HSV);

	split(HSV, planes);
	//���õ��������ʾ���Ԫ��

	MatIterator_<uchar> it_H = planes[0].begin<uchar>(),
						it_H_end = planes[0].end<uchar>();
	//MatIterator_<uchar> it_Cr = planes[2].begin<uchar>();
	MatIterator_<uchar> it_skin = dst.begin<uchar>();

	//�˵�Ƥ����ɫ��YCbCrɫ�ȿռ�ķֲ���Χ:100<=Cb<=127, 138<=Cr<=170
	for( ; it_H != it_H_end;  ++it_H, ++it_skin)
	{
		if ((7 <= *it_H) && (*it_H <= 29))
			*it_skin = 255;
		else
			*it_skin = 0;
	}
	//���ͺ͸�ʴ�����Ϳ�������������ѷ��Žӣ�����ʴ��������ϸ��͹�𣨡��ߵ㡱������
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
	//ת��ΪYCrCb��ɫ�ռ�
	cvtColor(src, YCbCr, CV_RGB2YCrCb);
	//����ͨ��ͼ�����Ϊ�����ͨ��ͼ��
	split(YCbCr, planes);

	//���õ��������ʾ���Ԫ��

	MatIterator_<uchar> it_Cb = planes[1].begin<uchar>(),
						it_Cb_end = planes[1].end<uchar>();
	MatIterator_<uchar> it_Cr = planes[2].begin<uchar>();
	MatIterator_<uchar> it_skin = dst.begin<uchar>();

	//�˵�Ƥ����ɫ��YCbCrɫ�ȿռ�ķֲ���Χ:100<=Cb<=127, 138<=Cr<=170
	for( ; it_Cb != it_Cb_end; ++it_Cr, ++it_Cb, ++it_skin)
	{
		if (138 <= *it_Cr &&  *it_Cr <= 170 && 100 <= *it_Cb &&  *it_Cb <= 127)
			*it_skin = 255;
		else
			*it_skin = 0;
	}

	//���ͺ͸�ʴ�����Ϳ�������������ѷ��Žӣ�����ʴ��������ϸ��͹�𣨡��ߵ㡱������
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


