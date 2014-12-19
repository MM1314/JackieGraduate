package com.example.mousecontrol;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.LoaderCallbackInterface;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public class MControlService extends Service {
	static final String TAG = "MControl Service";
	DetectHandView mView;
	
	private BaseLoaderCallback mOpenCVCallBack = new BaseLoaderCallback(this){
		@Override 
		public void onManagerConnected(int status){
			switch(status){
			case LoaderCallbackInterface.SUCCESS:
				Log.i(TAG, "Opencv loaded successfully");
				System.loadLibrary("detection_based_tracker");
				
				mView = new DetectHandView(mAppContext);
				//mView.setDetectorType(mDetectorType);
			}
		}
	};

	@Override
	public IBinder onBind(Intent arg0) {
		// TODO Auto-generated method stub
		return null;
	}
	
	@Override
	public void onCreate(){
		super.onCreate();
		System.out.println("Service is created");
	}
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId){
		System.out.println("Service is started");
		return START_STICKY;
	}
	
	@Override
	public void onDestroy(){
		super.onDestroy();
		System.out.println("Service is Destoryed");
		
	}

}
