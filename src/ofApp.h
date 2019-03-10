#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "camThread.h"
#include "timerThread.h"
#include "ofxHttpUtils.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
//		ofVideoGrabber vidGrabber;
//		ofPixels videoInverted;
//		ofTexture videoTexture;
//		int camWidth;
//		int camHeight;
//		int lightnessVal = 150;
//		// ofTexture       camtex;
//		// ofPixels p;
//		ofTexture       camtex;
		int offsetX;
		int offsetY;
		int margin = 5;
		void yesButtonClicked();
		void noButtonClicked();
		bool bClicked = false;
		ofxButton yesButton;
		ofxButton noButton;
		ofImage img;
		MyTimer mTimer;
		CamThread cam;
		unsigned long counter = 0;
		vector<pair<string, int>> results;
		vector<pair<int, int>> currentFactors;
		vector<vector<pair<int, int>>> factors;
		vector<string> questions;
		string currentQuestion;
		ofRectangle r;
		string notification;
		ofRectangle rNotification;
		int count = 0;
	
		void newResponse(ofxHttpResponse & response);
		ofxHttpUtils httpUtils;
		string responseStr;
		string requestStr;
		string action_url;
};
