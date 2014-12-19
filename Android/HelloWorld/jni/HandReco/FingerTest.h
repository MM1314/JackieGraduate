#include <opencv2/core/core.hpp>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

/*
 * 手势识别：主要包括对手指个数的确定，以及Hu的求解
 * 步骤：先找出中心，通过几何特征来确定手指个数
 * 在通过Hu距的相似性做出判断，确定最终的手势
 * */
class FingerTest{
public:
	FingerTest(){}
//	FingerTest(){}
	//确定手指个数，并比较Hu距的相似性
	int findFingers(Mat& src);
	//找出二值化图像的轮廓，并进行相应的处理
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
