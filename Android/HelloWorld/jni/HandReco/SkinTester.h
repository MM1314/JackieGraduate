
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

/*SkinTester类提供了不同颜色空间的肤色检验方法。
 * 具体包括HSV，YCrCb,OSTU 等不同的方法；
 *
 * 最终生成二值图像，并进行形态学的相应处理
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

	//HSV中 7<H<29
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
