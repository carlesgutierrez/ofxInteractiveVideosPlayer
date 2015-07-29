//
//  InteractiveVideosPlayer.h
//  
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

//GUI
#include "ofxXmlSettings.h"
#include "ofxUI.h"
#include "ofxUIDefines.h"

//OSC
#include "ofxOsc.h"

#include "statsRecorder.h"

#define PORT_OSC_RECEIVE 16000
#define PORT_OSC_SENDER 10000
#define TIME_GOOD_INTERACTION 70

enum modeInteraction {
	modeSequence = 0,
	modeRandom,
	modeCover
};

class ofxInteractiveVideosPlayer {
public:
	ofxInteractiveVideosPlayer();
	~ofxInteractiveVideosPlayer();
	
	void setup(string path);
	void setMainVideo(bool _bMainVideo);
	void update();
	void draw();
	void exit();
	
	void Jump2NextAction();
	
	////VIDEO INTERATIONS
	void resetType2(modeInteraction mode);
	void setPosition(float pos);
	void play();
 	void pause();
	void next();
	void prev();
	int toggleMainVideo();



#ifdef USE_HAPCODEC
	vector<ofxHapPlayer> videos;
#else
	vector<ofVideoPlayer> videos;
#endif
	
	///////////
	//ofApp events
	void keyReleased(int key);
	void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
	void mouseMoved(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	//Stats
	bool bDrawDebuggin = false;
	statsRecorder   statsMouse;
	
	//Basic Gui
	ofxUICanvas *gui;
	void createGUI();
	ofVec2f posGui;
	void guiEventBasics(ofxUIEventArgs &e);
	
private:
	
	void setupAllNormalVideos(string _videopath);
	void jump2MainVideo();
	void jump2InitVideo();
	void jump2NextVideo();
	void jump2PrevVideo();
	void jump2RandomVideo();
	void setFramePosition(int framePos);

	////////////////
	//control vars
	modeInteraction myInteractionType;
	bool bMainVideo;
	bool bScratching;
	//Video Control vars
	int idVidNow = -1;
	bool bScratchingActive = false;
	bool bReady4VideoInteraction = false;
	bool firstPressedActionDone = false;
	long counterFramesInteraction = 0;
	int currentFrame, currentPressedFrame, totalFramesVideo = 0;
	vector<bool> activeVideos;
	
	///////////
	//Directories, menus and future subMenus
	ofDirectory dir;
	int numVideos = 0;
	string videosPath;
	int idMainVideo = 0;
	
	///////////
	//control skratching
	float pressedVideoPosition = 0.0;
	float posMousePressedInVid;
	int bDetectedEndVideo = 0;
	
	//////////////
	///Visual FeedBacks
	ofImage myFeedBackCursorImage;
	bool bShowHandFeedBack;
	bool bShowVideoFeedBack;

	
	///////////
	//OSC
	void setupReceiverOSC();
	void updateReceiverOSC();
	void setupSenderStadisticsOSC();
	
	
	ofxOscSender sender;
	ofxOscReceiver receiver;
	float oscMouseX = 0.0f;
	float oscMouseY = 0.0f;
	float oscMouseInside = 0.0f;
	void updateMouseEvents(float _oscMouseX, float _oscMouseY, float _oscMouseInside);
	void sendStadisticsOSC(int timePlayingPerVideo);
	
	
};