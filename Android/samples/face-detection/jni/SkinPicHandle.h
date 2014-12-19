#include <cv.h>
#include <highgui.h>
#include <cxcore.h>

using namespace cv;
using namespace std;


//#define IMAGE_EMPTY -1
int skinRGB(cv::Mat &image, float *radio);

bool detectAndDraw( cv::Mat& img, CascadeClassifier& cascade, double scale, cv::Rect &result);

