/**
* Copyright (C) 2013 Imran Akthar (www.imranakthar.com)
* imran@imranakthar.com
*/
package com.example.handtrack;

import java.util.List;
import java.util.Vector;

import org.opencv.core.Mat;
import org.opencv.core.Rect;
import org.opencv.core.RotatedRect;

public class TrackedObj
{
	//hsv, hue, mask, prob
	/*利用Camshifting算法，所以需要hsv,hue空间
	 * 以及掩模和prob(不知道是什么)
	 * 
	 * */
	Mat hsv,hue,mask,prob;
	//预测矩形框
	Rect prev_rect;
	RotatedRect  curr_box;
	//http://stackoverflow.com/questions/10324727/opencv-how-to-convert-a-cpp-code-in-c
	//颜色直方图 
	Mat hist;
	public List<Mat> hsvarray,huearray;

	public TrackedObj()
	{
	//hsv=new Mat();
	//hue=new Mat();
	//mask=new Mat();
	//prob=new Mat();
	hist=new Mat();
	prev_rect=new Rect();
	curr_box=new RotatedRect();
	hsvarray=new Vector<Mat>();
	huearray=new Vector<Mat>();
	
	}
}
