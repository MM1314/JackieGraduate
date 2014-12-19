package com.example.myhand;

public class HelloFloat {
	static native float myPrint();
	static {
		System.loadLibrary("helloFloat");
	}
	
	public void print(){
		float radio = myPrint();
		System.out.println(radio);
	}	
}
