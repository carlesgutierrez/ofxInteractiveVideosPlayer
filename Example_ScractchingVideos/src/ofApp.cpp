#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
   
	ofSetFrameRate(30);
    ofBackground(50);
    
    #ifdef USE_MTL_MAPPING
    _mapping = new ofxMtlMapping2D();
    _mapping->init(ofGetWidth(), ofGetHeight(), "mapping/xml/shapes.xml", "mapping/controls/mapping.xml");
	#endif
	
#ifdef USE_HAPCODEC
	myInteractiveVideos.setup("videos/codecHAP/");
#else
	myInteractiveVideos.setup("videos/codecQT/");
#endif
	
	ofHideCursor();
	bMouseHide = true;
}
			
//--------------------------------------------------------------
void ofApp::update(){
	
    #ifdef USE_MTL_MAPPING
	_mapping->update();
	#endif
	
	myInteractiveVideos.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(255, 255, 255);
	
	// ----
	#ifdef USE_MTL_MAPPING
	_mapping->bind();
	#endif
    
	myInteractiveVideos.draw();
    
	#ifdef USE_MTL_MAPPING
	_mapping->unbind();
    
    //-------- mapping of the towers/shapes
    _mapping->draw();
	#endif
	
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == OF_KEY_RETURN){
		ofToggleFullscreen();
		bMouseHide = !bMouseHide;
		if(bMouseHide)ofHideCursor();
		else ofShowCursor();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

	myInteractiveVideos.keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	myInteractiveVideos.mouseDragged(x,y,button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	myInteractiveVideos.mousePressed(x,y,button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	myInteractiveVideos.mouseReleased(x,y,button);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit(){
	myInteractiveVideos.exit();
}


