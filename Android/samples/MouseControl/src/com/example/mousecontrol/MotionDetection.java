package com.example.mousecontrol;

import java.util.ArrayDeque;
import java.util.Deque;

import org.opencv.core.Point;

import android.util.Log;


enum Direction{NONE, LEFT, RIGHT}

/*DisplayMetrics metrics =new DisplayMetrics();
WindowManager WM = (WindowManager)context.getSystemService(Context.WINDOW_SERVICE);
Display display = WM.getDefaultDisplay();
display.getMetrics(metrics);
int height = metrics.heightPixels; //屏幕高
int width = metrics.widthPixels;    //屏幕的宽
*/
public class MotionDetection {
	private static final String   TAG = "Sample::MotionDetection";
	public int POSTHRESHOLD = 200;
	public int MULDISTANCE = 100;
	public int QUEUE_SIZE = 10;
	
	private int ppDistance;
	private int disTimes;
	private int revDire;
	private Direction lastDireX;
	private Direction dire;
	
	private Point lastPoint;
	
	private Deque<Point> posQueue;
	public String msg ;
	public void init(){
		Log.i(TAG, "init");
		lastDireX = Direction.NONE;
		dire = Direction.NONE;
		ppDistance = 0;
		disTimes = 1;
		revDire = 0;
		msg = new String();
	}
	public MotionDetection(){
		init();
		posQueue = new ArrayDeque<Point>();
	}
	
	//POSTHRESHOLD
	public void setPosThreshold(int length){
		this.POSTHRESHOLD = length/3;
		this.MULDISTANCE = length/5;
	}

	private void add(Point point){
		//Log.i(TAG, "add posQueue ");
		if(posQueue.isEmpty() || 
		   (posQueue.size() > 3 && Math.abs(lastPoint.x - point.x) > MULDISTANCE)){
			posQueue.clear();
			//if(posQueue.isEmpty())
				//Log.i(TAG, "the pos is empty");
			//else if (Math.abs(lastPoint.x - point.x) > MULDISTANCE)
				//Log.i(TAG, "the two points' distance is so large~~");
			//Log.i(TAG, "clear the posQueue ");
			init();	
			lastPoint = null;
		}
		else
			lastPoint = posQueue.getLast();

		posQueue.addLast(point);		
		if(posQueue.size() > QUEUE_SIZE)
			resize();
	}
	
	private void setDirection(Point point){
		if(posQueue.size() == 1)
			lastDireX = Direction.NONE;
			//return Direction.NONE;
		
		else if(posQueue.size() == 2){
			revDire = 0;
			if(posQueue.getFirst().x < posQueue.getLast().x){
				lastDireX = Direction.LEFT;
			}
			else{
				lastDireX  = Direction.RIGHT;
			}
		}
	}
	
	public boolean handle(Point point){
		add(point);
		setDirection(point);
		
		Log.i(TAG, "posQueue's length is " + posQueue.size());
		
		if(posQueue.size() <= 2)
			return false;
		
		if(lastDireX == Direction.LEFT && lastPoint.x < point.x ||
				lastDireX == Direction.RIGHT && lastPoint.x > point.x){
			ppDistance += Math.abs(point.x - lastPoint.x);
			revDire = 0;
			Log.i(TAG, "direction" + lastDireX + " ppDistance :" + ppDistance);
		}
		else{
			revDire++;
			if(revDire >= 5)
				changeDirection();
			return false;
		}
		
		if(ppDistance > this.POSTHRESHOLD){
			msg = new String();
			msg = "Direction" + lastDireX + " Distance"
						 + ppDistance + " Times:" + disTimes;
			dire = lastDireX;
			disTimes++;
			ppDistance -= POSTHRESHOLD;
			
			Log.i(TAG, msg);
			return true;
		}
		return false;
		
	}
	
	public void changeDirection(){
		int index = posQueue.size() - revDire;
		while(index > 0){
			posQueue.peekFirst();
			index--;
			//Log.i(TAG, "peek the posQueue ");
		}
		if(posQueue.getFirst().x < posQueue.getLast().x)
			lastDireX = Direction.LEFT;
		else
			lastDireX = Direction.RIGHT;
		
		Log.i(TAG, "change the posQueue's direction ");
		revDire = 0;
	}
	
	public void resize(){
		//Log.i(TAG, "resize the posQueue ");
		if(posQueue.size() > QUEUE_SIZE){
			int index =  QUEUE_SIZE;
			while(index > 0)
				posQueue.peekFirst();
				index--;
		}
	}
}
