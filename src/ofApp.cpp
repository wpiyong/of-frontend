#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
//	camWidth = 320;  // try to grab at this size.
//	camHeight = 240;
//
//	//get back a list of devices.
//	vector<ofVideoDevice> devices = vidGrabber.listDevices();
//
//	for(size_t i = 0; i < devices.size(); i++){
//		if(devices[i].bAvailable){
//			//log the device
//			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
//		}else{
//			//log the device and note it as unavailable
//			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
//		}
//	}
//
//	vidGrabber.setDeviceID(0);
//	vidGrabber.setDesiredFrameRate(60);
//	vidGrabber.initGrabber(camWidth, camHeight);
//
//	videoInverted.allocate(camWidth, camHeight, OF_PIXELS_RGB);
//	videoTexture.allocate(videoInverted);
	ofSetVerticalSync(true);
	
	action_url = "http://localhost:3000/question/questions?type=common";
	ofAddListener(httpUtils.newResponseEvent,this,&ofApp::newResponse);
	ofxHttpForm form;
	form.action = action_url;
	form.method = OFX_HTTP_GET;
	httpUtils.addForm(form);

	httpUtils.addForm(form);
	httpUtils.start();
	
	questions.push_back("Do you like apple?");
	questions.push_back("Do you like hiking?");
	questions.push_back("Do you like fishing?");
	
	results.push_back(pair<string, int>("A", 50));
	results.push_back(pair<string, int>("B", 87));
	results.push_back(pair<string, int>("C", 90));
	results.push_back(pair<string, int>("D", 30));
	
	for(int i = 0; i < questions.size(); i++){
		vector<pair<int, int>> factor;
		factor.push_back(pair<int, int>(5, -3));
		factor.push_back(pair<int, int>(2, -5));
		factor.push_back(pair<int, int>(-4, 2));
		
		factors.push_back(factor);
	}
	currentQuestion = questions[count];
	currentFactors = factors[count];
	ofBitmapFont f;
	r = f.getBoundingBox(currentQuestion, 0, 0);
	
	offsetX = ofGetWidth() / 3;
	offsetY = 0;
	
	yesButton.addListener(this, &ofApp::yesButtonClicked);
	yesButton.setup("Yes");
	yesButton.setPosition((ofGetWidth() + offsetX)/2 + 5, ofGetHeight()-100);
	yesButton.setSize(50, 20);
	
	noButton.addListener(this, &ofApp::noButtonClicked);
	noButton.setup("No");
	noButton.setPosition((ofGetWidth() + offsetX)/2 - 50 - 5, ofGetHeight()-100);
	noButton.setSize(50, 20);
	
	ofSetFrameRate(26);
	ofBackground(255, 255, 255);
	cam.interval   = 25; // in ms
	cam.deviceName = "Logitech Camera";
	
	cam.camWidth 		= 1280;
	cam.camHeight 		= 720;
}

void ofApp::newResponse(ofxHttpResponse & response){
	responseStr = ofToString(response.status) + ": " + (string)response.responseBody;
}

//--------------------------------------------------------------
void ofApp::update(){
//	ofBackground(100, 100, 100);
//	vidGrabber.update();
//
//	if(vidGrabber.isFrameNew()){
//		ofPixels & pixels = vidGrabber.getPixels();
//		for(size_t i = 0; i < pixels.size(); i++){
//			//invert the color of the pixel
//			videoInverted[i] = 255 - pixels[i];
//		}
//		//load the inverted pixels
//		videoTexture.loadData(videoInverted);
//	}
	if(cam.isThreadRunning()){
		cam.lock();
		
		// copy image
		img.setFromPixels(cam.getImage().getPixels());
		//img = cam.getImage();
		// done with the resource
		cam.unlock();
		if(mTimer.counter >= 6000 && counter == 0){
			counter = mTimer.counter;
			cam.bSave = true;
		}
		if(mTimer.counter > 9000){
//			mTimer.stopTimer();
//			cam.stop();
//			bClicked = false;
			count++;
			if(count >= questions.size()){
//				currentQuestion = "Thank you!";
//				ofBitmapFont f;
//				r = f.getBoundingBox(currentQuestion, 0, 0);
//				currentFactors = {{0,0},{0,0},{0,0},{0,0}};
				//timer > 15 second, restart the question
				if(mTimer.counter > 15000){
					mTimer.stopTimer();
					cam.stop();
					bClicked = false;
					count = 0;
					currentQuestion = questions[count];
					ofBitmapFont f;
					r = f.getBoundingBox(currentQuestion, 0, 0);
					currentFactors = factors[count];
				} else {
					currentQuestion = "Thank you!";
					ofBitmapFont f;
					r = f.getBoundingBox(currentQuestion, 0, 0);
					currentFactors = {{0,0},{0,0},{0,0},{0,0}};
				}
			} else {
				mTimer.stopTimer();
				cam.stop();
				bClicked = false;
				currentQuestion = questions[count];
				ofBitmapFont f;
				r = f.getBoundingBox(currentQuestion, 0, 0);
				currentFactors = factors[count];
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
//	ofSetHexColor(0xffffff);
//	vidGrabber.draw(20, 20);
//	videoTexture.draw(20 + camWidth, 20, camWidth, camHeight);
//
//	camtex.draw( 20,20 + camHeight, camWidth, camHeight);
	ofFill();
	ofSetColor(ofColor(100, 100, 140));
	ofDrawRectangle(margin, margin, offsetX - 2 * margin, ofGetHeight() - 2 * margin);
	
	ofSetColor(200,10,10);
	for(int i = 0; i < results.size(); i++){
		ofDrawBitmapString(results[i].first, margin * 2, 100 + i * 50 + i * margin + 25);
		ofFill();
		ofDrawRectangle(margin * 2 + 60, 100 + i * 50 + i * margin, results[i].second, 50);
	}
	
	ofSetColor(ofColor(150, 150, 190));
	
	// draw divide line
	ofDrawRectangle(offsetX, margin, 2, ofGetHeight() - 2 * margin);
	
	// draw right panel
	ofDrawRectangle(offsetX + margin, margin, ofGetWidth() - offsetX - 2 * margin, ofGetHeight() - 2 * margin);
	ofSetColor(10,10,10);
	// draw question
	if(!bClicked){
		ofDrawBitmapString(ofToString(count + 1) + "/" + ofToString(questions.size()) , ofGetWidth() - 80, 100);
		ofDrawBitmapString(currentQuestion, offsetX + (ofGetWidth() - offsetX - r.getWidth())/2, 150);
	}
//	ofSetColor(0);
//	ofDrawBitmapString("Framerate/Frames saved: " + ofToString((int)ofGetFrameRate()) + "/" + ofToString(cam.savedFrames), 20, 20);
//	ofDrawBitmapString("Status:", 20, 40);
//
//	if(cam.isThreadRunning()) {
//		ofSetColor(255, 0, 0);
//		ofDrawBitmapString("RECORDING", 80, 40);
//	} else {
//		ofDrawBitmapString("NOT RECORDING", 80, 40);
//	}
	// draw button
//	yesButton.draw();
//	noButton.draw();
//	ofDrawBitmapString(mTimer.counter, 500, 500);
	
	//draw capture image
	if(bClicked){
		ofSetColor(255,255,255);
		int w = ofGetWidth() - offsetX - 4 * margin;
		int h = 720 * w / 1280;
		img.draw(offsetX + margin, (ofGetHeight() - 100 - h) / 2 , ofGetWidth() - offsetX - 4 * margin, h);
		if(mTimer.counter < 5000){
			// smile
			notification = "Smiling ...";
		} else if(mTimer.counter < 7000){
			// capturing
			notification = "Capturing ...";
		} else{
			// finished
			notification = "Finished";
		}
		ofBitmapFont f;
		rNotification = f.getBoundingBox(notification, 0, 0);
		int yPosition = (ofGetHeight() - 100 - h) / 2 + h + 50;
		ofDrawBitmapString(notification, offsetX + (ofGetWidth() - offsetX - r.getWidth())/2, yPosition);
	} else {
		if(count < questions.size()){
			yesButton.draw();
			noButton.draw();
		}
	}
}

void ofApp::yesButtonClicked(){
	if(!bClicked){
		bClicked = true;
		mTimer.startTimer();
		counter = 0;
		cam.start();
		for(int i = 0; i < factors.size(); i ++){
			results[i].second +=  currentFactors[i].first;
		}
	} else {
		bClicked = false;
		mTimer.stopTimer();
	}
}

void ofApp::noButtonClicked(){
	if(!bClicked){
		bClicked = true;
		mTimer.startTimer();
		counter = 0;
		cam.start();
		for(int i = 0; i < factors.size(); i ++){
			results[i].second +=  currentFactors[i].second;
		}
	} else {
		bClicked = false;
		mTimer.stopTimer();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
//	if(key == 's' || key == 'S'){
//		vidGrabber.videoSettings();
//	}
//
//	if(key == 'g')
//	{
//		camtex.loadData(vidGrabber.getPixels());
//	}
	
//	if(key == 32) {
//		if(cam.isThreadRunning()) {
//			cam.stop();
//		} else {
//			cam.start();
//		}
//	}
//
//	//reset recording, start new folder w. r-key
//	if(key == 114) {
//		cam.reset();
//	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	offsetX = ofGetWidth() / 3;
	
	yesButton.setPosition((ofGetWidth() + offsetX)/2 + 5, ofGetHeight()-100);
	noButton.setPosition((ofGetWidth() + offsetX)/2 - 50 - 5, ofGetHeight()-100);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
