package com.example.myhand;

import java.util.List;
import java.util.Vector;

import org.opencv.core.Mat;
import org.opencv.core.Rect;
import org.opencv.core.RotatedRect;

public class TrackedObj {
	Mat hsv;
	Mat hue;
	Mat mask;
	Mat prob;
	
	Rect prev_rect;
	RotatedRect curr_box;
	Mat hist;
	public List<Mat> hsvarray;
	public List<Mat> huearray;
	
	public TrackedObj(){
		hist = new Mat();
		prev_rect = new Rect();
		curr_box = new RotatedRect();
		hsvarray = new Vector<Mat>();
		huearray = new Vector<Mat>();
	}
}
