package com.example.myhand;

import org.opencv.core.Mat;
import org.opencv.core.Point;

public class HandUtils {

	public static boolean skinTest(Mat mRgb){
	float radio = nativeSkinTest(mRgb);
	
	if(radio > 0.4)
		return true;
	else
		return false;
	}

	
	public static boolean MotionDetection(Point p){
		return false;
	}
	
private static float nativeSkinTest(Mat mRgb) {
		return nativeSkinTest(mRgb);
	}

	private static native float nativeSkinTest(long inputImage);
	
	static
	{
		System.loadLibrary("hand_utils");
	}
}
