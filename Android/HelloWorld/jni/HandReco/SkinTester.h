
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

/*SkinTester���ṩ�˲�ͬ��ɫ�ռ�ķ�ɫ���鷽����
 * �������HSV��YCrCb,OSTU �Ȳ�ͬ�ķ�����
 *
 * �������ɶ�ֵͼ�񣬲�������̬ѧ����Ӧ����
 * */
enum{
	HSV = 1,
	YCC,
	OSTU,
	OTHRES,
};
class SkinTester{
public:
//	static const int HSV
//	static const int YCC = 2;
//	static const int OSTU = 3;
//	static const int OTHERS = 4;

	//HSV�� 7<H<29
	SkinTester(){};
	void skinHSV(const Mat& src, Mat& dst);

	Mat skinHSV(const Mat& src);

	void skinYCC(const Mat& src, Mat& dst);

	Mat skinYCC(const Mat& src);

	void skinOSTU(const Mat& src, Mat& dst);
	Mat skinOSTU(const Mat& src);

	void skinRGB(const Mat& src, Mat& dst);

	void skinOther(const Mat& src, Mat& dst);

	void skin(int TYPE, const Mat& src, Mat& dst);

	~SkinTester(){};
};
