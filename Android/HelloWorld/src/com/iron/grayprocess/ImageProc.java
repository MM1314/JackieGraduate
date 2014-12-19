package com.iron.grayprocess;

import android.util.Log;

//_com_iron_grayprocess2_ImageProc
public class ImageProc {
	public static final String TAG = "Jackie Test";
	static {
		Log.i(TAG, "begin to load GrayPic Lib~~");
		System.loadLibrary("GrayPic");
	}

	public static native int[] grayProc(int[] pixels, int w, int h);

	public static native int[] procPic(int[] pixels, int w, int h);
}
