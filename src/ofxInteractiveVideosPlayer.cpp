//
//  ofxInteractiveVideosPlayer.cpp
//  
//
//  Created by carles on 26/02/15.
//
//

#include "ofxInteractiveVideosPlayer.h"

ofxInteractiveVideosPlayer::ofxInteractiveVideosPlayer(){

}

ofxInteractiveVideosPlayer::~ofxInteractiveVideosPlayer() {
	bMainVideo = false;
}



//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::setup(string _videopath){
	
	setupAllNormalVideos(_videopath);

	jump2InitVideo();
	
	ofSeedRandom();
	
	// listen on the given port
	cout << "listening for osc messages on port " << PORT_OSC_RECEIVE << "\n";
	
	//Load a visual cursor for feedback
	myFeedBackCursorImage.loadImage("FeedBackImages/HandWithTransparency.png");
	myFeedBackCursorImage.setAnchorPoint(myFeedBackCursorImage.getWidth()*0.5, myFeedBackCursorImage.getHeight()*0.5);
	
	//OSC
	setupReceiverOSC();
	setupSenderStadisticsOSC();
	
	//Stats //TODO REMOVE this and use smooth movements
	statsMouse.setup(1,0,ofGetWidth());
	statsMouse.addMarker( 10  );
	
	
	//Default mode. After setup is able to resetMode to another one more complex
	myInteractionType = modeSequence;
	
	//Load from XML
	//bShowVideoFeedBack = false;
	//bShowHandFeedBack = false;
	
	
	//GUI
	gui = NULL;
	createGUI();

}

//--------------------------------------------------------------
int ofxInteractiveVideosPlayer::toggleMainVideo(){
	bMainVideo = !bMainVideo;
	return (int)bMainVideo;
}

//TODO
//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::resetType2(modeInteraction _mode){
	
	myInteractionType = _mode;
	
	//reset the mode. From actual to the next mode.
	switch (myInteractionType) {
		case modeSequence:
			//TODO
			break;
		case modeRandom:
			//TODO
			break;
		case modeCover:
			//TODO
			break;
		default:
			//Do nothing
			break;
	}
	
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
void ofxInteractiveVideosPlayer::setupAllNormalVideos(string _videopath){
	
	videosPath = _videopath;
	
	idVidNow = -1;
	
	dir.allowExt("mov");
	
	//List all videos inside the path folder
	dir.listDir(videosPath);
	
	numVideos = dir.size();
	cout << "numVideos found in path " << videosPath << "is = " << numVideos << endl;
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
// Update OSC commands && video/s && Check End Of Video
void ofxInteractiveVideosPlayer::update(){
	
	//Update OSC from external Interaction
	updateReceiverOSC();
	
	//update active video/s
	if(idVidNow > -1){
		
		switch (myInteractionType) {
			case modeSequence:
				//
				videos[idVidNow].update();
				
				break;
			case modeRandom:
				
				videos[idVidNow].update();
				
				break;
			case modeCover:
				//TODO Check and Update how many videos at same time do you want to play
				
				videos[idVidNow].update();
				
				break;
			default:
				//do nothing
				break;
		}
		
		
	}
	
	//Check END of VIDEOs. Take care If modeInteraction use a main video after end Actual Video
	if(idVidNow > -1){
		
		//Fisrt if actual video is almost finishing then...
		if(videos[idVidNow].getPosition() > 0.90){
			
			if(bMainVideo){
				if(!bScratchingActive && idMainVideo != idVidNow){
					jump2MainVideo();
				}else if(!bScratchingActive && idMainVideo == idVidNow){
					Jump2NextAction();
				}
			}else{
				if(!bScratchingActive){
					Jump2NextAction();
					//TODO. Just if you want to change video while scraching. Then its necessary to Reset methods for mouse and other video behauviours.
				}
			}

			
		}//END VIDEO

	}
	else{
		//NO VIDEO/S NO FUN
	}
	

	//if(idVidNow > -1)cout << "videos[" << idVidNow << "].getPosition() = " << videos[idVidNow].getPosition() << endl;
	
	
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::draw(){
	
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
	if(idVidNow > -1){
		
		
		ofSetColor(255, 255, 255);
		videos[idVidNow].draw(0, 0, ofGetWidth(), ofGetHeight());
		
		
		if(bShowVideoFeedBack == true){
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
	
	ofSetColor(ofColor::white);
	if(bShowHandFeedBack){
		if(ofGetMousePressed()){
			myFeedBackCursorImage.draw(ofVec2f(ofGetMouseX(), ofGetMouseY()), myFeedBackCursorImage.getWidth(), myFeedBackCursorImage.getHeight());
		}
	}
	
}


//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::Jump2NextAction(){

	switch (myInteractionType) {
		case modeSequence:
			//
			jump2NextVideo();
			break;
		case modeRandom:
			//TODO
			
			break;
		case modeCover:
			//TODO
			
			break;
		default:
			//do nothing
			break;
	}

}


//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::play(){}
//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::pause(){}
//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::next(){}
//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::prev(){}

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
void ofxInteractiveVideosPlayer::jump2InitVideo(){
	
	//actual position
	if(idVidNow > -1){
		idVidNow = 0;
		videos[idVidNow].play();
		videos[idVidNow].setPosition(0);
	}
	
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
			videos[idVidNow].setPosition(0);
			videos[idVidNow].play();
		}else{videos[idVidNow].play();}
	}

}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::jump2PrevVideo(){
	
	//actual position
	if(idVidNow > -1){
		videos[idVidNow].stop();
		videos[idVidNow].setPosition(0);
		
		//next position
		idVidNow--;
		if (idVidNow < 0) {
			idVidNow = numVideos -1;
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
void ofxInteractiveVideosPlayer::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::keyReleased(int key){

	if( key == OF_KEY_RIGHT ){
		jump2NextVideo();
	}
	else if(key == OF_KEY_LEFT){
		jump2PrevVideo();
	}
	else if(key == 'r'){
		jump2RandomVideo();
	}else if(key == OF_KEY_F1){
		bDrawDebuggin = !bDrawDebuggin;
	}
	else if(key == 'g' || key == 'G'){
		gui->toggleVisible();
	}
		
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::mouseDragged(int x, int y, int button){
	
	//Process X to be progressive value
	
	statsMouse.update( x );
	counterFramesInteraction++;
	//cout << "counterFramesInteraction = " << counterFramesInteraction << endl;
	
	//float posMouseInVid = ofMap(x, 0, ofGetWidth(), 0, 1); // invert values to
	int auxLastValsAvg = 1;
	float posMouseInVid = ofMap(statsMouse.getAverage(auxLastValsAvg)*ofGetWidth(), 0, ofGetWidth(), 0, 1); // invert values to
	
	if(idVidNow > -1){
		
		float deltaMouseScratch = posMousePressedInVid - posMouseInVid;
		
		float posScratchingVid = pressedVideoPosition - deltaMouseScratch;
		
		int newFramePos = (int)(totalFramesVideo * posScratchingVid);
		//cout << "mouseDragged newFramePos = " << newFramePos << endl;
		
		//if(newFramePos > totalFramesVideo-totalFramesVideo*0.001){
		//	newFramePos = totalFramesVideo-totalFramesVideo*0.001;
		//}
		
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
	
	bScratchingActive = false;

	//Reset average interaction
	statsMouse.reset();
	
	if(idVidNow > -1){
			videos[idVidNow].play();
			videos[idVidNow].setSpeed(1);
		}
	
	//Extra behauviour in case bMainVideo Activated and we are already in it
	if(bMainVideo && idVidNow == idMainVideo){
		
		Jump2NextAction();
		
		cout << "Jump to video #" <<  idVidNow << endl;
	}
	
	//reset
	pressedVideoPosition = 0;

	
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::exit(){
	for(int i=0; i< numVideos; i++ ){
		videos.pop_back();
	}
	
	gui->saveSettings("guivideos/guiSettings.xml");
	cout << "saving Gui params to Xml" << endl;
}

//--------------------------------------------------------------
void ofxInteractiveVideosPlayer::createGUI(){
	
	posGui = ofVec2f(0, 0);
	gui = new ofxUICanvas(posGui.x, posGui.y, OFX_UI_GLOBAL_CANVAS_WIDTH, OFX_UI_GLOBAL_CANVAS_WIDTH);
	gui->addLabelToggle("Toggle Main Video", &bMainVideo);
	gui->addLabelToggle("Show Hand FeedBack", &bShowHandFeedBack);
	gui->addLabelToggle("Show Video FeedBack", &bShowVideoFeedBack);
	
	gui->autoSizeToFitWidgets();
	ofAddListener(gui->newGUIEvent,this,&ofxInteractiveVideosPlayer::guiEventBasics);
	gui->loadSettings("guivideos/guiSettings.xml");
	
}

//--------------------------------------------------------
void ofxInteractiveVideosPlayer::guiEventBasics(ofxUIEventArgs &e){
	string name = e.widget->getName();
	//	int kind = e.widget->getKind();
	if (name == "Toggle Main Video"){
		//bVisibleBasicTerrain = !bVisibleBasicTerrain;
		cout << "GUI Toggle Main Video  = "  << bMainVideo << endl;
	}
	else if (name == "Toggle Hand Video"){
		//bVisibleBasicTerrain = !bVisibleBasicTerrain;
		cout << "GUI Toggle Hand Video  = "  << bShowHandFeedBack << endl;
	}
	else if (name == "Toggle Video Video"){
		//bVisibleBasicTerrain = !bVisibleBasicTerrain;
		cout << "GUI Toggle Video Video  = "  << bShowVideoFeedBack << endl;
	}
}
