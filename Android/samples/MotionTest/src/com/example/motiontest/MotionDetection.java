package com.example.motiontest;

import java.util.ArrayDeque;
import java.util.Deque;

//import org.opencv.core.Point;
import org.opencv.core.Point;


enum Direction{NONE, LEFT, RIGHT}

/*DisplayMetrics metrics =new DisplayMetrics();
WindowManager WM = (WindowManager)context.getSystemService(Context.WINDOW_SERVICE);
Display display = WM.getDefaultDisplay();
display.getMetrics(metrics);
int height = metrics.heightPixels; //屏幕高
int width = metrics.widthPixels;    //屏幕的宽
*/
public class MotionDetection {
	public int POSTHRESHOLD = 50;
	public int MULDISTANCE = 35;
	public int QUEUE_SIZE = 30;
	
	private int ppDistance;
	private int disTimes;
	private int revDire;
	private Direction lastDireX;
	private Direction dire;
	
	private Point lastPoint;
	
	private Deque<Point> posQueue;
	public String msg ;
	public void init(){
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
		if(posQueue.isEmpty() || 
		   (posQueue.size() > 3 && Math.abs(lastPoint.x - point.x) > MULDISTANCE)){
			posQueue.clear();
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
		
		if(posQueue.size() <= 2)
			return false;
		
		if(lastDireX == Direction.LEFT && lastPoint.x < point.x ||
				lastDireX == Direction.RIGHT && lastPoint.x > point.x){
			ppDistance += Math.abs(point.x - lastPoint.x);
			revDire = 0;
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
			return true;
		}
		return false;
		
	}
	
	public void changeDirection(){
		int index = posQueue.size() - revDire;
		while(index > 0){
			posQueue.peekFirst();
		}
		if(posQueue.getFirst().x < posQueue.getLast().x)
			lastDireX = Direction.LEFT;
		else
			lastDireX = Direction.RIGHT;
		
		revDire = 0;
	}
	
	public void resize(){
		if(posQueue.size() > QUEUE_SIZE){
			int index =  QUEUE_SIZE;
			while(index > 0)
				posQueue.peekFirst();
		}
	}
}
