//#include "PositionJudge.h"

#include <opencv/cv.h>
#include <deque>
using namespace cv;
using namespace std;
enum Direction{NONE, LEFT, RIGHT,UP,DOWN};
const int QUEUE_SIZE = 30;

class PositionJudge{
public:
	PositionJudge():POSTHRESHOLD(50), MULDISTANCE(35){init();}
	~PositionJudge(){/*~this.posQueue;*/}

	void init();
	void handle(CvPoint p);
	deque<CvPoint> posQueue;
private:

	Direction lastDireX ;
	Direction lastDireY ;
	Direction dire ;

	const int POSTHRESHOLD;
	//const int POSTHRFF = 5;
	const int MULDISTANCE;
	int ppDistance;
	int disTimes;
	int revDire;
};

