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
	//�Լ��׵ķ�ʽѰ������������
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
	 //Ѱ������
	 Point center(moment.m10/moment.m00, moment.m01/moment.m00);
	 circle(show_img, center, 8 ,Scalar(0, 0, 255), CV_FILLED);

	 //�õ��������
	 //Rect_����Щ��˼����Ա����x��y��width��height���ֱ�Ϊ���Ͻǵ������;��εĿ��͸ߡ�
	 Rect rect = boundingRect(contours[index]);

	 //Ѱ��ָ��
	 vector<Point> couPoint = contours[index];
	 vector<Point> fingerTips;
	 Point tmp;
	 int max(0) , count(0), notice(0);
	 int minY = (int)((rect.y - center.y) / 3);
	 //ѭ����������������еĸ�����
	 for (int i = 0; i < couPoint.size(); i++)
	 {
		tmp = couPoint[i];
		int dist = (tmp.x - center.x) * (tmp.x - center.x) + (tmp.y - center.y) * (tmp.y - center.y);
		if (dist > max)
		{
			max = dist;
			notice = i;
		}
		// �������ֵ���ֵĵ������������40�����ֵ��Ҫ���ã����������maxֵ�����ã�
		// ���ǲ��ɹ�����֪��Ϊ�Σ�����ô����Ϊ�����ָ��
		if (dist != max)
		{
			count++;
			if (count > 40)
			{
				count = 0;
				max = 0;
				bool flag = false;
				// �������ĵĵ㲻��
				if (center.y < couPoint[notice].y )
					continue;
				// ���̫���Ĳ���
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
	//����ֱ�ӵ��ñ���ķ���
	findContour(src);
	return -1;
}