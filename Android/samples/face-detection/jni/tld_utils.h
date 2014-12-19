#include <opencv2/opencv.hpp>
#include <SkinPicHandle.h>
#include <stdio.h>
#include <linux/input.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#pragma once


void drawBox(cv::Mat& image, CvRect box, cv::Scalar color = cvScalarAll(255), int thick=1); 

void drawPoints(cv::Mat& image, std::vector<cv::Point2f> points,cv::Scalar color=cv::Scalar::all(255));

cv::Mat createMask(const cv::Mat& image, CvRect box);

inline float median(std::vector<float> v){
		float tmp = v.size() * 1.f/2;
	    //int n = floor((v.size() / 2.f));
		int n = floor(tmp);
	    nth_element(v.begin(), v.begin()+n, v.end());
	    return v[n];
}

std::vector<int> index_shuffle(int begin,int end);

void simulate_key(int fd, int kval);

void simulate_mouse(int fd, int rel_x, int rel_y);


//Rect findRect(Mat &image);

