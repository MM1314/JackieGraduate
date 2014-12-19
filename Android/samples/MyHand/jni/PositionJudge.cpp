#include <PositionJudge.h>

void PositionJudge::init(){
	lastDireX = NONE;
	lastDireY = NONE;
	dire = NONE;
	ppDistance = 0;
	disTimes = 1;
	revDire = 0;
}

void PositionJudge::handle(CvPoint point){
//取队尾元素
	CvPoint lastPoint ;
	if(posQueue.empty() == true){
		init();
		posQueue.push_back(point);
		return;		
	}

	lastPoint = posQueue.back();
	posQueue.push_back(point);
	//若长度为2，需为lastDire赋值
	if(posQueue.size() == 2){
		if(lastPoint.x < point.x){
			lastDireX = LEFT;
			revDire = 0;
		}else if(lastPoint.x >= point.x){
			lastDireX = RIGHT;
			revDire = 0;		
		}

	}
	//when the deque's size >= 3
	else{
		if(abs((lastPoint.x - point.x)) > MULDISTANCE){
			//clear(posQueue);
			posQueue.clear();
			init();
			posQueue.push_back(point);
			return;
		}
		
		//left
		else if(lastDireX == LEFT && lastPoint.x < point.x){
			ppDistance += (point.x - lastPoint.x);
		}//end for left 

		//right
		else if(lastDireX == RIGHT && lastPoint.x > point.x){
			ppDistance += (lastPoint.x - point.x);
		}//end for right
		else {
			revDire++;
			if(revDire >= 5){
			   posQueue.erase(posQueue.begin(), posQueue.begin() + (posQueue.size() - 6));
			   ppDistance = abs(posQueue.at(0).x -  posQueue.at(posQueue.size()-1).x);
			   if((posQueue.at(0).x -  posQueue.at(posQueue.size()-1).x) > 0)
				lastDireX = RIGHT;
			   else
				lastDireX = LEFT;
			   revDire = 0;
			}
			return;
		}

		if(ppDistance > POSTHRESHOLD){
		     cout<<"Dierction"<<lastDireX<<"\tDistance:"<<ppDistance <<"\tTimes:"<<disTimes<<endl;
			dire = lastDireX;
			disTimes++;
			ppDistance -= POSTHRESHOLD;
	
		}
	}//end for else

	if(posQueue.size() > QUEUE_SIZE){
		posQueue.erase(posQueue.begin(), posQueue.begin() + (posQueue.size() - 5));
	}

}
