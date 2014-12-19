package com.example.helloworld;

//import org.opencv.android.BaseLoaderCallback;
//import org.opencv.android.LoaderCallbackInterface;
//import org.opencv.android.OpenCVLoader;

import com.iron.grayprocess.ImageProc;
import com.jackie.utils.CONSTANTS;

//import com.example.helloworld.R;
import android.os.Bundle;
import android.os.Handler;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Bitmap.Config;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;

/**/
public class MainActivity extends Activity implements OnClickListener {

	public static final String TAG = "JackieTest";
	private Button btnProc;
	private ImageView imageView;
	private Bitmap bmp;
	//add by Jackie on 20141211
	private int DURATION = 1000;
	Handler handler = new Handler();
	Runnable runnable = new Runnable(){
		@Override
		public void run() {
			try{
				//handler.postDelayed(this, DURATION);
				sendControl(1);
				System.out.println("have send the broadcast~~");
			}catch(Exception e){
				e.printStackTrace();
				System.out.println("some exception occurs~~");
			}
		}};

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		btnProc = (Button) findViewById(R.id.btn_gray_process);
		imageView = (ImageView) findViewById(R.id.image_view);
		// 将lena图像加载程序中并进行显示
		bmp = BitmapFactory.decodeResource(getResources(), R.drawable.one);
		imageView.setImageBitmap(bmp);
		btnProc.setOnClickListener(this);
		
		System.out.println("begin to post it~~");
		handler.postDelayed(runnable, DURATION);
	}

	@Override
	public void onClick(View v) {
		synchronized(bmp){
			int w = bmp.getWidth();
			int h = bmp.getHeight();
			// 将图片转化为int数组类型，再进行相关的处理
			int[] pixels = new int[w * h];
			bmp.getPixels(pixels, 0, w, 0, 0, w, h);
			// 使用JNI函数进行处理，并返回处理后图像数组（输入与输出均为整型数组）
			//int[] resultInt = ImageProc.grayProc(pixels, w, h);
			int[] resultInt = ImageProc.procPic(pixels, w, h);
			
			//Bitmap resultImg = Bitmap.createBitmap(w, h, Config.ARGB_8888);
			Bitmap resultImg = Bitmap.createBitmap(w, h, Config.RGB_565);
			resultImg.setPixels(resultInt, 0, w, 0, 0, w, h);
			imageView.setImageBitmap(resultImg);
		}
		
//		if(bmp.isRecycled()){
//			bmp.recycle();
//			bmp = null;
//			System.gc();
//		}
	}

	@Override
	public void onResume() {
		super.onResume();
		// 通过OpenCV引擎服务加载并初始化OpenCV类库，所谓OpenCV引擎服务即是
		// OpenCV_2.4.3.2_Manager_2.4_*.apk程序包，存在于OpenCV安装包的apk目录中
		// OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_3, this,
		// mLoaderCallback);
	}
	
	//发送广播
	public void sendControl(int type){
		Intent intent = new Intent();
		intent.putExtra(CONSTANTS.INT_TYPE, type);
		intent.setAction(CONSTANTS.ACTION_NAME);
		sendBroadcast(intent);
	}
}
