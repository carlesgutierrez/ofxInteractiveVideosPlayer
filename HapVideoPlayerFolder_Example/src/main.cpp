#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1680,1050,OF_FULLSCREEN);			// <-------- setup the GL context
	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN , OF_WINDOW
	// pass in width and height too:
	ofRunApp(new ofApp());
	
}
