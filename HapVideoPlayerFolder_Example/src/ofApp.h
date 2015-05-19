#pragma once

#include "ofMain.h"

#ifdef USE_MTL_MAPPING
#include "ofxMtlMapping2D.h"
#endif

#include "ofxInteractiveVideosPlayer.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
		
	////////////////
	//Mapping
	#ifdef USE_MTL_MAPPING
	ofxMtlMapping2D* _mapping;
	#endif

	ofxInteractiveVideosPlayer	cagaTio;

	bool bMouseHide = true;
	
};
