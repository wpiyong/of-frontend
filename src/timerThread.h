//
//  timerThread.h
//  myCam
//
//  Created by Yong Liu on 3/8/19.
//

#ifndef timerThread_h
#define timerThread_h

#include "ofMain.h"
class MyTimer : public ofThread {
	public:
	unsigned long counter = 0;
	
	MyTimer() {
		timer.setPeriodicEvent(1000000); // 1 second
	}
	void resetTimer(){
		counter = 0;
		timer.reset();
	}
	void startTimer(){
		startThread();
	}
	void stopTimer(){
		resetTimer();
		stopThread();
	}
	private:
	ofTimer timer;
	void threadedFunction() {
		while (isThreadRunning()) {
			timer.waitNext();
			
			counter ++;
		}
	}
};
#endif /* timerThread_h */
