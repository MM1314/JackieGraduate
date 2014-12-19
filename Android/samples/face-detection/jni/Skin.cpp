#include "Skin.h"

void skinRGB(Mat &image, float *radio){
	if(!image.data){
		printf("The Mat Image is Empty~~\n");
		//return -1;
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
				  int index = 0;
				  while(index <3){
					  *pdst = *prgb;
					  pdst++;
					  prgb++;
					  index++;
				  }

				SkinPixelSum++;
			}


		}
	}

	//resize(dst_tmp, dst_tmp,dsize);
	*radio = SkinPixelSum * 1.0 /(image.rows * image.cols);
	printf("%f\n", radio);

}
