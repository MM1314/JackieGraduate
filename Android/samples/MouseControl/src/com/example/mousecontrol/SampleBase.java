package com.example.mousecontrol;

import java.util.List;

import org.opencv.core.Size;
import org.opencv.highgui.Highgui;
import org.opencv.highgui.VideoCapture;
import android.graphics.Bitmap;
import android.util.Log;

//主要进行摄像头的相应管理，包括 openCamera, releaseCamera的相关操作
//以及FpsMeter的相关显示
public abstract class SampleBase implements Runnable{
	  private static final String TAG = "Sample::SampleBase";
	    private VideoCapture        mCamera;
	    private FpsMeter            mFps;
	    
	    public SampleBase(){
	    	mFps = new FpsMeter();
	        Log.i(TAG, "Instantiated new " + this.getClass());
	        (new Thread(this)).start();
	    }
	    
	    public boolean openCamera(){
	    	Log.i(TAG, "openCamera");
	    	synchronized(this){
	    		releaseCamera();
	    		mCamera = new VideoCapture(Highgui.CV_CAP_ANDROID);
	    		if(mCamera.isOpened() == false){
	    			mCamera.release();
	    			mCamera = null;
	    			Log.e(TAG, "Failed to open native camera");
	    			return false;
	    		}
	    	}
	    	return true;
	    }
	    
	    public void releaseCamera(){
	    	Log.i(TAG, "releaseCamera");
	    	synchronized(this){
	    		if(mCamera != null){
	    			mCamera.release();
	    			mCamera = null;
	    		}
	    	}
	    	
	    }

	    protected abstract Bitmap processFrame(VideoCapture capture);
	    
	    public void setupCamera(int width, int height) {
	        Log.i(TAG, "setupCamera("+width+", "+height+")");
	        synchronized (this) {
	            if (mCamera != null && mCamera.isOpened()) {
	                List<Size> sizes = mCamera.getSupportedPreviewSizes();
	                int mFrameWidth = width;
	                int mFrameHeight = height;
	                // selecting optimal camera preview size
	                {
	                    double minDiff = Double.MAX_VALUE;
	                    for (Size size : sizes) {
	                        if (Math.abs(size.height - height) < minDiff) {
	                            mFrameWidth = (int) size.width;
	                            mFrameHeight = (int) size.height;
	                            minDiff = Math.abs(size.height - height);
	                        }
	                    }
	                }

	                mCamera.set(Highgui.CV_CAP_PROP_FRAME_WIDTH, mFrameWidth);
	                mCamera.set(Highgui.CV_CAP_PROP_FRAME_HEIGHT, mFrameHeight);
	            }
	        }

	    }

		@Override
		public void run() {
			// TODO Auto-generated method stub
	          Bitmap bmp = null;
	          while(true){
	            synchronized (this) {
	                if (mCamera == null)
	                    break;

	                if (!mCamera.grab()) {
	                    Log.e(TAG, "mCamera.grab() failed");
	                    break;
	                }

	                bmp = processFrame(mCamera);

	                mFps.measure();
	            }
	            Log.i(TAG, "Finishing processing thread");
	          }
		}
}
