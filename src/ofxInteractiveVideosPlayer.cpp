//
//  ofxInteractiveVideosPlayer.cpp
//  myChinoCagaTio
//
//  Created by carles on 26/02/15.
//
//

#include "ofxInteractiveVideosPlayer.h"

ofxInteractiveVideosPlayer::ofxInteractiveVideosPlayer(){

}

ofxInteractiveVideosPlayer::~ofxInteractiveVideosPlayer() {

}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::setup(){
	
	setupAllNormalVideos();

	jump2NextVideo();
	
	ofSeedRandom();
	
	// listen on the given port
	cout << "listening for osc messages on port " << PORT_OSC_RECEIVE << "\n";
	
	//OSC
	setupReceiverOSC();
	setupSenderStadisticsOSC();
	
	//Stats
	statsMouse.setup(10,0,ofGetWidth());
	statsMouse.addMarker( 100  );
}
//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::setupSenderStadisticsOSC(){
//	/selected_product label
	sender.setup("localhost", PORT_OSC_SENDER);
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::setupReceiverOSC(){
	receiver.setup(PORT_OSC_RECEIVE);
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::setupAllNormalVideos(){
	
	idVidNow = -1;
	
	dir.allowExt("mov");
	
#ifdef USE_HAPCODEC
	dir.listDir("videos/codecHAP/");
#else
	dir.listDir("videos/normal/");
#endif
	
	numVideos = dir.size();
	cout << "numVideos found = " << numVideos << endl;
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
	

	//Assign values to vector variables
	
#ifdef USE_HAPCODEC
		videos.assign(dir.size(), ofxHapPlayer());
#else
		videos.assign(dir.size(), ofVideoPlayer());
#endif
	
		//TODO videosbPlayed as a control variable to know how was already showed
		//videosNotPlayed.assign(dir.size()-1, 0); //integers better than boolean (boolean caused fatal error)
	
	
	for(int i=0; i< videos.size(); i++ ){
		cout << "getPath(i)=" << dir.getPath(i) << endl;
		videos[i].loadMovie(dir.getPath(i));

		cout << "videos[i].getWidth() = " << videos[i].getWidth() << endl;		cout << "videos[i].getHeight() = " << videos[i].getHeight() << endl;
		cout << "videos[i].getTotalNumFrames=" << videos[i].getTotalNumFrames() << endl;

		videos[i].stop();
		videos[i].setVolume(0);
		cout << "loaded new chino " << endl;
		
		//Vector to control who is not being shown. From 1 to the end. // O is reserved to the main video
		//if(i != 0)videosNotPlayed[i] = i;
	}
	
	if(videos.size() > 0){
		idVidNow = 0;
	}
	
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::update(){
	
	
	updateReceiverOSC();

	
	if(idVidNow > -1){

		videos[idVidNow].update();

		if(videos[idVidNow].getPosition() > 0.989 && !bScratchingActive){

			jump2MainVideo();
			


			//reset
			firstPressedActionDone = false;
		}
		else if(videos[idVidNow].getPosition() > 0.989 && bScratchingActive){
			bDetectedEndVideo++;
		}
	}
	
	if (bDetectedEndVideo > 1) {
		bDetectedEndVideo = 0;
		jump2MainVideo();
		//reset
		firstPressedActionDone = false;
		cout << "Auto jump2MainVideo" << endl;
	}

	//if(idVidNow > -1)cout << "videos[" << idVidNow << "].getPosition() = " << videos[idVidNow].getPosition() << endl;
	
	
	
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::draw(){
	
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
	if(idVidNow > -1){
		

		ofSetColor(255, 255, 255);
		videos[idVidNow].draw(0, 0, ofGetWidth(), ofGetHeight());
		
		
		if(bScratchingActive == true){
			ofSetColor(ofColor::steelBlue); //200, 15, 15
			ofFill();
			float widthBar = 0.03;
			ofRect(0, ofGetHeight() - ofGetHeight()*widthBar, videos[idVidNow].getPosition()*ofGetWidth(), ofGetHeight()*widthBar);
		}
		
		if(bDrawDebuggin){
			float posx = (ofGetWidth()-ofGetWidth()*0.3);
			float posy = (ofGetHeight()-ofGetHeight()*0.2);
			statsMouse.draw(posx,posy,200,200,10, "Interaction X axis", true, 20);
		}
		
	}
	
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::sendStadisticsOSC(int idVideo){
	ofxOscMessage m;
	m.setAddress("/selected_product");
	
	m.addIntArg(idVideo);
	
	sender.sendMessage( m );
	
	//Ideally send the amount of interaction if we analise all interactions. Now only sending when de cualitative interaction has been done.
	//ofxOscMessage m2;
	//m2.setAddress("/activity");
	//m2.addIntArg(counterFramesInteraction);
	//sender.sendMessage( m2 );
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::updateReceiverOSC(){
	
	bool bOscMessage = false;
	float _oscMouseX = 0.0f;
	float _oscMouseY = 0.0f;
	float _oscMouseInside = 0.0f;
	
	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		
		// check for mouse moved message
		if(m.getAddress() == "/laserRangeData"){
			// get the arguments
			_oscMouseX = m.getArgAsFloat(0);
			_oscMouseY = m.getArgAsFloat(1);
			_oscMouseInside = m.getArgAsFloat(2);
			
			bOscMessage = true;
		}
	}
	
	//Then send Mouse Event if ther was an OSC mouse event
	
	if (bOscMessage) {
		updateMouseEvents(_oscMouseX, _oscMouseY, _oscMouseInside);
	}
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::updateMouseEvents(float _oscMouseX, float _oscMouseY, float _oscMouseInside){
	
	float interactiveMouseX = ofMap(_oscMouseX, -1, 1, 0, ofGetWidth());
	float interactiveMouseY = ofMap(_oscMouseY, -1, 1, 0, ofGetHeight());
	
	if (_oscMouseInside != oscMouseInside && _oscMouseInside == 0) {
		//mouse released
		mouseReleased(interactiveMouseX, interactiveMouseY, 0);
		
	}else if(_oscMouseInside != oscMouseInside && _oscMouseInside == 1){
		//mouse pressed
		mousePressed(interactiveMouseX, interactiveMouseY, 0);
		
	}else if(_oscMouseInside == oscMouseInside && _oscMouseInside == 1){
		//mouse dragged
		mouseDragged(interactiveMouseX, interactiveMouseY, 0);
	}else{
		//no action
		cout << "No Action, This waste CPU" << endl;
	}
	
	//save last values
	oscMouseX = _oscMouseX;
	oscMouseY = _oscMouseY;
	oscMouseInside = _oscMouseInside;
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::jump2NextVideo(){
	
	//actual position
	if(idVidNow > -1){
		videos[idVidNow].stop();
		videos[idVidNow].setPosition(0);
		
		//next position
		idVidNow++;
		if (idVidNow > numVideos-1) {
			idVidNow = 0;
			videos[idVidNow].play();
		}else{videos[idVidNow].play();}
	}

}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::jump2MainVideo(){
	
	//SEND OSC selection
	
	if(counterFramesInteraction > TIME_GOOD_INTERACTION){
		sendStadisticsOSC(idVidNow);
		cout << "SendStadisticsOSC, idVidNow = " <<  idVidNow << endl;
		counterFramesInteraction = 0;
	}
	
	//Reset average interaction
	statsMouse.reset();
	
	//actual position
	if(idVidNow > -1){
		videos[idVidNow].stop();
		videos[idVidNow].setPosition(0);
		
		//Main video is 0 ( _000_xxxx )
		idVidNow = 0;
		
		if (idVidNow > numVideos-1) {
			idVidNow = 0;
		}
		
		//videos[idVidNow].setPosition(0);
		videos[idVidNow].setFrame(0);//default 0
		videos[idVidNow].update();
		videos[idVidNow].play();
	}
	
}

/* OLD
//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::jump2RandomVideo(){
	//actual position
	if(idVidNow > -1){
		videos[idVidNow].stop();
		videos[idVidNow].setPosition(0);
		//next position
		idVidNow++;
		idVidNow = ofClamp(ofRandom(numVideos), 1, numVideos);
		
		if (idVidNow > numVideos-1) {
			idVidNow = 0;
			videos[idVidNow].play();
		}else{videos[idVidNow].play();}
	}
	
}*/

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::jump2RandomVideo(){
	
	//Reset average interaction
	counterFramesInteraction = 0;
	statsMouse.reset();
	
	//actual position
	if(idVidNow > -1){
		videos[idVidNow].stop();
		videos[idVidNow].setPosition(0);
		//next position
		idVidNow++;
		idVidNow = (int)(ofRandom(1, numVideos));

		if (idVidNow > numVideos-1) {
			idVidNow = 0;
			videos[idVidNow].play();
		}else{videos[idVidNow].play();}
	}
	
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::setPosition(float pos){
	if(idVidNow > -1){
		videos[idVidNow].stop();//Slow FrameRate
		videos[idVidNow].setPosition(pos);//Slow FrameRate
	}
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::setFramePosition(int framePos){
	if(idVidNow > -1){
		videos[idVidNow].stop();//Slow FrameRate
		videos[idVidNow].setFrame(framePos);
	}
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::keyReleased(int key){

	
	if( key == 'n' ){
		jump2NextVideo();
	}else if(key == 'r'){
		jump2RandomVideo();
	}else if(key == OF_KEY_F1){
		bDrawDebuggin = !bDrawDebuggin;
	}
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::mouseDragged(int x, int y, int button){
	
	//Process X to be progressive value
	
	statsMouse.update( x );
	counterFramesInteraction++;
	//cout << "counterFramesInteraction = " << counterFramesInteraction << endl;
	
	//float posMouseInVid = ofMap(x, 0, ofGetWidth(), 0, 1); // invert values to
	int auxLastValsAvg = 100;
	float posMouseInVid = ofMap(statsMouse.getAverage(auxLastValsAvg)*ofGetWidth(), 0, ofGetWidth(), 0, 1); // invert values to
	
	if(idVidNow > -1){
		
		float deltaMouseScratch = posMousePressedInVid - posMouseInVid;
		
		float posScratchingVid = pressedVideoPosition - deltaMouseScratch;
		
		int newFramePos = (int)(totalFramesVideo * posScratchingVid);
		//cout << "mouseDragged newFramePos = " << newFramePos << endl;
		setFramePosition(newFramePos);
		
	}
	
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::mousePressed(int x, int y, int button){

	//Reset average interaction
	statsMouse.reset();
	
	if(idVidNow > -1){
		
		//get actual video position
		pressedVideoPosition = videos[idVidNow].getPosition();
		//get actual mouse position
		posMousePressedInVid = ofMap(x, 0, ofGetWidth(), 0, 1);
		
		currentPressedFrame = videos[idVidNow].getCurrentFrame();
		cout << "currentPressedFrame = " << currentPressedFrame << endl;
		totalFramesVideo = videos[idVidNow].getTotalNumFrames();
		cout << "totalFramesVideo = " << totalFramesVideo << endl;
		//videos[idVidNow].setSpeed(0.01);
		
		bScratchingActive = true;
		
	}
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::mouseReleased(int x, int y, int button){
	
	//Reset average interaction
	statsMouse.reset();
	
	if(idVidNow > -1){
			videos[idVidNow].play();
			videos[idVidNow].setSpeed(1);
		}
	
		bScratchingActive = false;
	
	//Go to check for the following mousePressed action to set interactive video mode
	if(idVidNow == 0){
			bReady4VideoInteraction = true;
		
	}
	
	if(bReady4VideoInteraction && !firstPressedActionDone){
		firstPressedActionDone = true;
		jump2RandomVideo();
		cout << "Jump to random video" << endl;
	}
	
	//reset
	pressedVideoPosition = 0;

	
	
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::exit(){
	for(int i=0; i< numVideos; i++ ){
		videos.pop_back();
	}
}
