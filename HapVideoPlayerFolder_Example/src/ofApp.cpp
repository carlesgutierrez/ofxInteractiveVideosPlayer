#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
   
	ofSetFrameRate(30);
    ofBackground(50);
    
    #ifdef USE_MTL_MAPPING
    _mapping = new ofxMtlMapping2D();
    _mapping->init(ofGetWidth(), ofGetHeight(), "mapping/xml/shapes.xml", "mapping/controls/mapping.xml");
	#endif
	
	cagaTio.setup();
	
	ofHideCursor();
	bMouseHide = true;
}
			
//--------------------------------------------------------------
void ofApp::update(){
	
    #ifdef USE_MTL_MAPPING
	_mapping->update();
	#endif
	
	cagaTio.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(255, 255, 255);
	
	// ----
	#ifdef USE_MTL_MAPPING
	_mapping->bind();
	#endif
    
	cagaTio.draw();
    
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

	cagaTio.keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	cagaTio.mouseDragged(x,y,button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	cagaTio.mousePressed(x,y,button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	cagaTio.mouseReleased(x,y,button);
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
	cagaTio.exit();
}


