#include <opencv2/opencv.hpp>
#include <tld_utils.h>
#include <PositionJudge.h>
#include <iostream>
#include <sstream>
#include <TLD.h>
#include <stdio.h>
using namespace cv;
using namespace std;
//Global variables

PositionJudge pj;
const String cascadeName = "../xml/palm.xml";//手掌的训练数据
Rect box;
bool drawing_box = false;
bool gotBB = false;
bool tl = true;
bool rep = false;
bool fromfile=false;
string video;
CascadeClassifier cascade;//创建级联分类器对象
double scaleHaar = 1.3;
bool autoSelect = false;
Rect rect;
bool flag = false;


void readBB(char* file){
  ifstream bb_file (file);
  string line;
  getline(bb_file,line);
  istringstream linestream(line);
  string x1,y1,x2,y2;
  getline (linestream,x1, ',');
  getline (linestream,y1, ',');
  getline (linestream,x2, ',');
  getline (linestream,y2, ',');
  int x = atoi(x1.c_str());// = (int)file["bb_x"];
  int y = atoi(y1.c_str());// = (int)file["bb_y"];
  int w = atoi(x2.c_str())-x;// = (int)file["bb_w"];
  int h = atoi(y2.c_str())-y;// = (int)file["bb_h"];
  box = Rect(x,y,w,h);
}

void findRect(Mat &image){
	if( !image.empty() )//读取图片数据不能为空
		{
			flag = detectAndDraw( image, cascade, scaleHaar, rect);
			if(flag == true){
				gotBB = true;
				drawing_box = false;
				box = rect;
			}
			waitKey(100);
		}
}


void read_options(int argc, char** argv,VideoCapture& capture,FileStorage &fs){
  for (int i=0;i<argc;i++){
      if (strcmp(argv[i],"-b")==0){
          if (argc>i){
              readBB(argv[i+1]);
              gotBB = true;
          }
      }
      if (strcmp(argv[i],"-s")==0){
          if (argc>i){
              video = string(argv[i+1]);
              capture.open(video);
              fromfile = true;
          }

      }
      if (strcmp(argv[i],"-p")==0){
          if (argc>i){
              fs.open(argv[i+1], FileStorage::READ);
          }
      }
      if (strcmp(argv[i],"-no_tl")==0){
          tl = false;
      }
      if (strcmp(argv[i],"-r")==0){
          rep = true;
      }
  }
}

int main(int argc, char * argv[]){
  VideoCapture capture;
  capture.open(0);
  FileStorage fs;
  //Read options
  read_options(argc,argv,capture,fs);
  //Init camera
  if (!capture.isOpened())
  {
	cout << "capture device failed to open!" << endl;
   	return 1;
  }
  if( !cascade.load( cascadeName ) )//从指定的文件目录中加载级联分类器
  {
	cerr << "ERROR: Could not load classifier cascade" << endl;
	return 2;
  }

  TLD tld;
  //Read parameters file
  tld.read(fs.getFirstTopLevelNode());
  Mat frame;
  Mat last_gray;
  Mat first;
  int index = 0;
  if (fromfile){
	/**/do{
		capture >> frame;
		findRect(frame);
		imshow("TLD", frame);
	}while(gotBB == false);

      capture >> frame;
      cvtColor(frame, last_gray, CV_RGB2GRAY);
      frame.copyTo(first);
  }else{
      capture.set(CV_CAP_PROP_FRAME_WIDTH,340);
      capture.set(CV_CAP_PROP_FRAME_HEIGHT,240);
  }

  ///Initialization
GETBOUNDINGBOX:
  while(!gotBB) // gotBB == false
  {
    if (!fromfile){ // fromFile == false
      capture >> frame;
    }
    else
      first.copyTo(frame);

    cvtColor(frame, last_gray, CV_RGB2GRAY);
    drawBox(frame,box);
    imshow("TLD", frame);
    if (cvWaitKey(33) == 'q')
	    return 0;
  }

    if(gotBB == true){
	drawBox(frame, box);
	imshow("TLD", frame);
	if(cvWaitKey(33) == 'q')
		return 0;	
    }
	cout<<"min(box.width,box.height):"<<min(box.width,box.height);
	cout<<"\tgetFirstTopLevelNode()[min_win]: "<<(int)fs.getFirstTopLevelNode()["min_win"]<<endl;
  if (min(box.width,box.height)<(int)fs.getFirstTopLevelNode()["min_win"]){
	
      cout << "Bounding box too small, try again." << endl;
      gotBB = false;
      goto GETBOUNDINGBOX;
  }
  //Remove callback
  if(drawing_box == true)
  	cvSetMouseCallback( "TLD", NULL, NULL );

  printf("Initial Bounding Box = x:%d y:%d h:%d w:%d\n",box.x,box.y,box.width,box.height);
  //Output file
  FILE  *bb_file = fopen("bounding_boxes.txt","w");
  //TLD initialization
  tld.init(last_gray,box,bb_file);

  ///Run-time
  Mat current_gray;
  BoundingBox pbox;
  BoundingBox tbox;
  vector<Point2f> pts1;
  vector<Point2f> pts2;
  bool status=true;
  int frames = 1;
  int detections = 1;
REPEAT:
  while(capture.read(frame)){
    //get frame
    cvtColor(frame, current_gray, CV_RGB2GRAY);
    //Process Frame
    tld.processFrame(last_gray,current_gray,pts1,pts2,pbox,status,tl,bb_file);
    //Draw Points
    	
    if (status){
      drawPoints(frame,pts1);
      drawPoints(frame,pts2,Scalar(0,255,0));
      drawBox(frame,pbox);
      detections++;
      //center = cvPoint((pbox.x+pbox.width)/2, (pbox.y + pbox.height)/2);
      //pj.handle(center);
    }	
	
    //Display
    detectAndDraw( frame, cascade, scaleHaar, rect);
    rectangle(frame, rect,CV_RGB(0, 0, 255),2);
    imshow("TLD", frame);
    //swap points and images
    swap(last_gray,current_gray);
    pts1.clear();
    pts2.clear();
    frames++;
   // printf("Detection rate: %d/%d\n",detections,frames);
    if (cvWaitKey(33) == 'q')
      break;
    if(cvWaitKey(30) == 'p')
	cvWaitKey(0);
  }
  if (rep){
    rep = false;
    tl = false;
    fclose(bb_file);
    bb_file = fopen("final_detector.txt","w");
    //capture.set(CV_CAP_PROP_POS_AVI_RATIO,0);
    capture.release();
    capture.open(video);
    goto REPEAT;
  }
  fclose(bb_file);
  return 0;
}
