package com.example.mousecontrol;

/*
 * 找到手的条件：应包含皮肤区域40%以上，面积条件可以放宽，不要过小就好
 * 
 * 查找逻辑：先使用javaDetector方法进行查找，若首次找到（满足相关条件）置found为true
 * 
 * 找到后进行跟踪，如果跟踪后没有找到(没有满足相关条件)，则found为false，将该图像交给
 * 
 * javaDetector进行处理；
 * 
 * 接下来需要处理的是手势的判断以及手势轨迹的判断
 * 
 *
 * */
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import org.opencv.android.Utils;
import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.RotatedRect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.highgui.Highgui;
import org.opencv.highgui.VideoCapture;
import org.opencv.imgproc.Imgproc;
import org.opencv.objdetect.CascadeClassifier;
import org.opencv.objdetect.Objdetect;





import android.content.Context;
import android.graphics.Bitmap;
import android.util.Log;
import android.view.SurfaceHolder;
import android.widget.Toast;

class FdView extends SampleCvViewBase {
    private static final String   TAG = "Sample::MyHandView";
    private Mat                   mRgba;
    private Mat                   mHsv;
    private Mat                   mGray;

    private File                mCascadeFile;
    private CascadeClassifier   mJavaDetector;
    private DetectionBasedTracker mNativeDetector;
    

    private static final Scalar   HAND_RECT_COLOR = new Scalar(0, 255, 0, 255);
    private static final Scalar HAND_RECT_CAMSHIFT_COLOR = new Scalar(0, 0, 255, 255);
    
    public static final int       JAVA_DETECTOR     = 0;
    public static final int       NATIVE_DETECTOR   = 1;
    private int                   mDetectorType     = JAVA_DETECTOR;
    private float                 mRelativeFaceSize = 0;
    private int					  mAbsoluteFaceSize = 0;
    
    public int isHeadPoseOn = 0;
    
    public boolean nextFrame = false;
    public boolean found = false;
    
    CamShifting cs;
    Rect[] handsRect;
    public int handArea = 0;
    
	public MotionDetection mDete;
    
    public void setMinFaceSize(float faceSize)
    {
    	//相对大小
		mRelativeFaceSize = faceSize;
		//绝对大小
		mAbsoluteFaceSize = 0;
    }
    
    public void setDetectorType(int type)
    {
    	if (mDetectorType != type)
    	{
    		mDetectorType = type;
    		
    		if (type == NATIVE_DETECTOR)
    		{
    			Log.i(TAG, "Detection Based Tracker enabled");
    			mNativeDetector.start();
    		}
    		else
    		{
    			Log.i(TAG, "Cascade detector enabled");
    			mNativeDetector.stop();
    		}
    	}
    }

    public FdView(Context context) {
        super(context);
        try {
        	cs = new CamShifting();
        	handArea = 0;
        	mDete = new MotionDetection();
            InputStream is = context.getResources().openRawResource(R.raw.fist);
            File cascadeDir = context.getDir("cascade", Context.MODE_PRIVATE);
            mCascadeFile = new File(cascadeDir, "fist.xml");
            FileOutputStream os = new FileOutputStream(mCascadeFile);

            byte[] buffer = new byte[4096];
            int bytesRead;
            while ((bytesRead = is.read(buffer)) != -1) {
                os.write(buffer, 0, bytesRead);
            }
            is.close();
            os.close();
            
            mJavaDetector = new CascadeClassifier(mCascadeFile.getAbsolutePath());
            if (mJavaDetector.empty()) {
                Log.e(TAG, "Failed to load cascade classifier");
                mJavaDetector = null;
      
            } else
                Log.i(TAG, "Loaded cascade classifier from " + mCascadeFile.getAbsolutePath());

            mNativeDetector = new DetectionBasedTracker(mCascadeFile.getAbsolutePath(), 0);
            cascadeDir.delete();
            
        } catch (IOException e) {
            e.printStackTrace();
            Log.e(TAG, "Failed to load cascade. Exception thrown: " + e);
        }
    }
    
  
    
    public Rect[] HandDetect(Mat mRgba, Mat mGray){
    	if(this.mAbsoluteFaceSize == 0){
    		int height = mGray.rows();
    		if(Math.round(height * this.mRelativeFaceSize) > 0){
    			this.mAbsoluteFaceSize = Math.round(height * this.mRelativeFaceSize);
    		}
    		this.mNativeDetector.setMinFaceSize(mAbsoluteFaceSize);
    	}
    	
    	MatOfRect hands = new MatOfRect();
    	
    	if(mDetectorType == JAVA_DETECTOR){
    		if(mJavaDetector != null){
    			mJavaDetector.detectMultiScale(mGray, hands, 1.1, 2, 2, 
    					new Size(this.mAbsoluteFaceSize, this.mAbsoluteFaceSize), new Size());
    		}
    	}
    	else if(mDetectorType == NATIVE_DETECTOR){
    		if(mNativeDetector != null){
    			mNativeDetector.detect(mGray, hands);
    		}
    	}
    	else{
    		Log.e(TAG, "Detection method is not selected~~");
    	}
    	
    	Rect[] handsArray = hands.toArray();
    	for(int i = 0; i < handsArray.length; ++i){
    		//if(HandUtils.skinTest(mRgb.submat(handsArray[i]))){
    		Mat temp = mRgba.submat(handsArray[i]);
    		//if(temp.cols() * temp.rows() > 100){
    			Core.rectangle(mRgba, handsArray[i].tl(), handsArray[i].br(), HAND_RECT_COLOR);
    		//}	
    	}
    	return handsArray;
    }
    
    @Override
	public void surfaceCreated(SurfaceHolder holder) {
        synchronized (this) {
            // initialize Mats before usage
            mGray = new Mat();
            mRgba = new Mat();
            mHsv = new Mat();
        }

        super.surfaceCreated(holder);
	}

    /*
     * 现有问题：对人脸的误检率比较大，要减少人脸干涉，可以使用面积，而且要加大
     * 使用特征检测方法
     * 拟采用方案：每次面积发生较大变化时就进行特征检测
     * */
	@Override
    protected Bitmap processFrame(VideoCapture capture) {
        capture.retrieve(mRgba, Highgui.CV_CAP_ANDROID_COLOR_FRAME_RGBA);
        capture.retrieve(mGray, Highgui.CV_CAP_ANDROID_GREY_FRAME);
        
        if(!nextFrame){//nextFrame = false
        	Log.i("HandView", "Calling create tracked object");
        	handsRect = HandDetect(mRgba, mGray);
        	if(handsRect.length > 0){
        		//for(int i = 0; i < handsRect.length; ++i){
        		for(int i = 0; i < 1; ++i){
        			Core.rectangle(mRgba, handsRect[i].tl(),  handsRect[i].br(), this.HAND_RECT_COLOR);
        			cs.create_tracked_object(mRgba, handsRect,cs);
        			this.handArea = handsRect[i].height * handsRect[i].width;
        			
        			Point center = new Point((handsRect[i].tl().x + handsRect[i].br().x)/2,
        					(handsRect[i].tl().y + handsRect[i].br().y)/2);
        			Log.i(TAG, "(x,y):" + center.x + "  " + center.y);

        			//mDete.handle(center);
        			//Log.i(TAG, mDete.msg);
        		}
        		nextFrame = true;
        	}
        }
        else{
        	RotatedRect hand_box = cs.camshift_track_face(mRgba, this.handsRect, cs);
        	int curr_area = hand_box.boundingRect().height * hand_box.boundingRect().width ;
        	//if(curr_area < 1.2 * handArea && curr_area > 0.7 * handArea){
        	if(curr_area < 3.5 * handArea && curr_area > 0.5 * handArea){
        		Core.ellipse(mRgba, hand_box, FdView.HAND_RECT_CAMSHIFT_COLOR, 6);
        		Core.rectangle(mRgba, hand_box.boundingRect().tl()
        				,hand_box.boundingRect().br(), HAND_RECT_CAMSHIFT_COLOR);
        		
        		Point center = new Point((hand_box.boundingRect().tl().x + hand_box.boundingRect().br().x)/2,
        									(hand_box.boundingRect().tl().y + hand_box.boundingRect().br().y)/2);
        		Log.i(TAG, "Tracked :(x,y):" + center.x + "  " + center.y);
      		
        		mDete.handle(center);
        		Log.i(TAG, mDete.msg);
        	}
        	else{
        		nextFrame = false;
        	}
        }
                
        Bitmap bmp = Bitmap.createBitmap(mRgba.cols(), mRgba.rows(), Bitmap.Config.ARGB_8888);
        try {
        	Utils.matToBitmap(mRgba, bmp);
        } catch(Exception e) {
        	Log.e(TAG, "Utils.matToBitmap() throws an exception: " + e.getMessage());
            bmp.recycle();
            bmp = null;
        }
        return bmp;
    }
	

    @Override
    public void run() {
        super.run();

        synchronized (this) {
            // Explicitly deallocate Mats
            if (mRgba != null)
                mRgba.release();
            if (mGray != null)
                mGray.release();
            if (mCascadeFile != null)
            	mCascadeFile.delete();
            if (mNativeDetector != null)
            	mNativeDetector.release();

            mRgba = null;
            mGray = null;
            mCascadeFile = null;
        }
    }
}
