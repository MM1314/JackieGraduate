#include "FingerTest.h"
#include "SkinTester.h"
Mat FingerTest::findContour(Mat& src){
	SkinTester tester;
	Mat dst, show_img;

	if(src.channels() == 4){
		//cvtColor(src, src, CV_RGBA2RGB);
		//CV_BGRA2RGB
		cvtColor(src, src, CV_BGRA2RGB);
	}

	dst.create(src.rows, src.cols, CV_8UC1);
	tester.skinYCC(src , dst);

	//return dst;


	src.copyTo(show_img, dst);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//以简易的方式寻找最外层的轮廓
	findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	int index;
    double area, maxArea(0);
	for (int i=0; i < contours.size(); i++)
	{
		area = contourArea(Mat(contours[i]));
		if (area > maxArea)
		{
			maxArea = area;
			index = i;
		}
	}

	if(maxArea < 100)
		return src;
	 Moments moment = moments(dst, true);
	 //寻找中心
	 Point center(moment.m10/moment.m00, moment.m01/moment.m00);
	 circle(show_img, center, 8 ,Scalar(0, 0, 255), CV_FILLED);

	 //得到外包矩阵
	 //Rect_类有些意思，成员变量x、y、width、height，分别为左上角点的坐标和矩形的宽和高。
	 Rect rect = boundingRect(contours[index]);

	 //寻找指尖
	 vector<Point> couPoint = contours[index];
	 vector<Point> fingerTips;
	 Point tmp;
	 int max(0) , count(0), notice(0);
	 int minY = (int)((rect.y - center.y) / 3);
	 //循环遍历最外层轮廓中的各个点
	 for (int i = 0; i < couPoint.size(); i++)
	 {
		tmp = couPoint[i];
		int dist = (tmp.x - center.x) * (tmp.x - center.x) + (tmp.y - center.y) * (tmp.y - center.y);
		if (dist > max)
		{
			max = dist;
			notice = i;
		}
		// 计算最大值保持的点数，如果大于40（这个值需要设置，本来想根据max值来设置，
		// 但是不成功，不知道为何），那么就认为这个是指尖
		if (dist != max)
		{
			count++;
			if (count > 40)
			{
				count = 0;
				max = 0;
				bool flag = false;
				// 低于手心的点不算
				if (center.y < couPoint[notice].y )
					continue;
				// 离得太近的不算
				for (int j = 0; j < fingerTips.size(); j++)
				{
					//changed by Jackie
					//if (abs(couPoint[notice].x - fingerTips[j].x) < 20)
					if (abs(couPoint[notice].x - fingerTips[j].x) < 200)
					{
						flag = true;
						break;
					}
				}
				if (flag) continue;
				fingerTips.push_back(couPoint[notice]);
				circle(show_img, couPoint[notice], 6 ,Scalar(0, 255, 0), CV_FILLED);
				line(show_img, center, couPoint[notice], Scalar(255, 0, 0), 2);
			}
		}
	}
//	 if(show_img.channels() == 3){
//		 cvtColor(show_img, show_img, CV_RGB2RGBA);
//	 }
	 return show_img;
	
	/* */
}

int FingerTest::findFingers(Mat& src){
	//可以直接调用本类的方法
	findContour(src);
	return -1;
}
