package com.example.mousecontrol;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.Button;

public class ControlActivity extends Activity {
	private static String TAG = "ControlActivity";
	public Intent serverIntent;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.i(TAG, "onCreate");
		serverIntent = new Intent(this, MControlService.class);
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.activity_control);
		Button start = (Button) findViewById(R.id.start);
		Button stop = (Button) findViewById(R.id.stop);
		
		start.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Log.i(TAG, "onStart");
				startService(serverIntent);
			}
		});
		
		stop.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Log.i(TAG, "onStop");
				stopService(serverIntent);
				System.exit(0);
			}
		} );
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.control, menu);
		return true;
	}

}
