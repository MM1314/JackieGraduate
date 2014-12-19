#include <SkinPicHandle.h>

int skinRGB(Mat &image, float *radio){
	if(!image.data){
		printf("The Mat Image is Empty~~\n");
		return -1;
	}

	Mat dst_tmp = Mat::zeros(image.size(), image.type());
	Size dsize = Size(500,500);

	static const int R = 2;
	static const int G = 1;
	static const int B = 0;
	int rows = image.rows;
	int cols = image.cols;

	int SkinPixelSum = 0;

	int r, g, b;
	uchar *prgb, *pdst;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			prgb = image.data + image.step[0] * i + image.step[1] * j;
			pdst = dst_tmp.data + dst_tmp.step[0] * i + dst_tmp.step[1] * j;
			r = *(prgb + R);
			g = *(prgb + G);
			b = *(prgb + B);
			  if((r>90 && g>35 && b>16 && r-b>10 && r-g>13)||
				(r>197 && g>207 && b>167 && abs(r-b)<=18 && r>b&& g>b)){
				memcpy(pdst,prgb,3);
				SkinPixelSum++;
			}
		}
	}
	resize(dst_tmp, dst_tmp,dsize);
	*radio = SkinPixelSum * 1.0 /(image.rows * image.cols);
}

bool detectAndDraw( Mat& img,
                   CascadeClassifier& cascade,
                   double scale, Rect &rectFit)
{
    int i = 0;
    double t = 0;
    vector<Rect> hands;

    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );//将图片缩小，加快检测速度

    cvtColor( img, gray, CV_BGR2GRAY );//因为用的是类haar特征，所以都是基于灰度图像的，这里要转换成灰度图像
    resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );//将尺寸缩小到1/scale,用线性插值
    equalizeHist( smallImg, smallImg );//直方图均衡

    cascade.detectMultiScale( smallImg, hands,
        1.1, 2, 0
        //|CV_HAAR_FIND_BIGGEST_OBJECT
//|CV_HAAR_DO_ROUGH_SEARCH
        |CV_HAAR_SCALE_IMAGE
        ,
        Size(30, 30) );

    
    float radio, radioFit = 0;
    Rect result = cvRect(0, 0, 0, 0);
    rectFit = cvRect(0, 0, 0, 0); 
    IplImage* temp  = NULL;
    bool flag = false;
    int x, y, w, h, area;

    for( vector<Rect>::const_iterator r = hands.begin(); r != hands.end(); r++, i++ )
    {
	radio = 0;
        Mat smallImgROI;
        Point center;
  
        int radius;
  
	x = cvRound(r-> x * scale);
	y = cvRound(r->y * scale);
	w = min(cvRound(r->width * scale), img.cols - x);
	h = min(cvRound(r->height * scale), img.rows - y);
	result = cvRect(x, y, w, h);
        area = r->width * r->height;
	/**/if(area > 1500){
		Mat imgROI(img, result);
                skinRGB(imgROI, &radio);
		if(radio >= 0.6){
		   flag =  true;
		   if(rectFit.width * rectFit.height <= area &&
			(radioFit < radio || abs(radioFit - radio) <= 0.2 )){
			radioFit = radio;
			rectFit = result;			
		   }
		}
         } 	
    }
	if(flag ){printf("%d\t%f\n", rectFit.width * rectFit.height, radioFit);}
    return flag;
}

