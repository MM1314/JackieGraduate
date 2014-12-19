package com.sky_dreaming.ACDsee;

import com.jackie.utils.CONSTANTS;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.view.KeyEvent;
import android.widget.Gallery;

public class GalBroadcastReceiver extends BroadcastReceiver {
	private Gallery gallery;
	public GalBroadcastReceiver(Gallery g){
		gallery = g;
	}
	@Override
	public void onReceive(Context context, Intent intent) {
		
		String action = intent.getAction();
		int result = intent.getIntExtra(CONSTANTS.INT_TYPE, -1);
		
		System.out.println("get the broadcast~~, the result is " + result);
		if(action.equals(CONSTANTS.ACTION_NAME)){
			int position = gallery.getSelectedItemPosition();
			int next = (position + 1) % gallery.getChildCount();
			int pre = (position - 1 + gallery.getChildCount()) % gallery.getChildCount();
			//进行相应的处理
			switch(result){
			case 0:
				break;
			case 1:
				//gallery.setSelection(next);
				gallery.onKeyDown(KeyEvent.KEYCODE_DPAD_RIGHT, null);
				break;
			case 2:
				gallery.setSelection(pre);
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			default:
				break;
				
			}

		}
	}

}
