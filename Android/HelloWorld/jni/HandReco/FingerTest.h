#include <opencv2/core/core.hpp>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

/*
 * ����ʶ����Ҫ��������ָ������ȷ�����Լ�Hu�����
 * ���裺���ҳ����ģ�ͨ������������ȷ����ָ����
 * ��ͨ��Hu��������������жϣ�ȷ�����յ�����
 * */
class FingerTest{
public:
	FingerTest(){}
//	FingerTest(){}
	//ȷ����ָ���������Ƚ�Hu���������
	int findFingers(Mat& src);
	//�ҳ���ֵ��ͼ�����������������Ӧ�Ĵ���
	Mat findContour(Mat& src);

	float matchHu(){
		return .0f;
	}
	~FingerTest(){};
	//const int matches[6][4] = {};

	/*	vector<Point> maxCou ;
	Moments moment;
	Mat src;
	Mat show_img;
	Mat mask;
	*/
};
