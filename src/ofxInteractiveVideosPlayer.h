//
//  InteractiveVideosPlayer.h
//  myChinoCagaTio
//
//  Created by carles on 26/02/15.
//
//

#pragma once

#include "ofMain.h"
#include "ExternalsConfig.h"

#ifdef USE_HAPCODEC
#include "ofxHapPlayer.h"
#endif

#include "ofxOsc.h"

#include "statsRecorder.h"

#define PORT_OSC_RECEIVE 16000
#define PORT_OSC_SENDER 10000
#define TIME_GOOD_INTERACTION 70

class ofxInteractiveVideosPlayer {
public:
	ofxInteractiveVideosPlayer();
	~ofxInteractiveVideosPlayer();
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyReleased(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	
	////////////////
	void setPosition(float pos);
	void setupAllNormalVideos();
	void jump2MainVideo();
	void jump2NextVideo();
	void jump2RandomVideo();
	void setFramePosition(int framePos);
	
	///OSC
	void setupReceiverOSC();
	void updateReceiverOSC();
	void setupSenderStadisticsOSC();
	
	//normal
	ofDirectory dir;
	int numVideos = 0;
	
#ifdef USE_HAPCODEC
	vector<ofxHapPlayer> videos;
#else
	vector<ofVideoPlayer> videos;
#endif
	
	//vector<int> videosNotPlayed;
	
	////////////////
	//Control vars
	int idVidNow = -1;
	bool bScratchingActive = false;
	bool bReady4VideoInteraction = false;
	bool firstPressedActionDone = false;
	long counterFramesInteraction = 0;
	int currentFrame, currentPressedFrame, totalFramesVideo = 0;
	
	//control skratching
	float pressedVideoPosition = 0.0;
	float posMousePressedInVid;
	int bDetectedEndVideo = 0;
	
	//OSC
	ofxOscSender sender;
	ofxOscReceiver receiver;
	float oscMouseX = 0.0f;
	float oscMouseY = 0.0f;
	float oscMouseInside = 0.0f;
	void updateMouseEvents(float _oscMouseX, float _oscMouseY, float _oscMouseInside);
	void sendStadisticsOSC(int timePlayingPerVideo);
	
	//Stats interaction
	bool bDrawDebuggin = false;
	statsRecorder   statsMouse;
	
	
};