package com.example.myhand;

import android.net.Uri;
import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.util.Log;
import android.view.View;
import android.widget.Button;


public class MHandScreen extends Activity {
	
	private static String TAG = "MScreen";
	public Intent serverintent;

	//@Override
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.i(TAG,"onCreate");
		serverintent = new Intent(this, MHandScreen.class);
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_mscreen);
		Button button1 = (Button)findViewById(R.id.Button01);
		Button button2 = (Button)findViewById(R.id.Button02);
		button1.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				Log.i(TAG,"onClick1");
				startService(serverintent);
			}
		});
		button2.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				Log.i(TAG,"onClick2");
				stopService(serverintent);
				System.exit(0);
			}
		});
	}

}
