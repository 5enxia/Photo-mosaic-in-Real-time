#pragma once

#include "ofMain.h"
#include "JsonManager.h"
#include "ofxPostGlitch.h"

class ofApp : public ofBaseApp {

public:
	//GENERAL
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	//RGB CAMERA
	ofVideoGrabber video;
	ofPixels video2pix;
	bool isCamStream;
	bool isCamDraw;
	int size = 48; 	//optimized s = 48
	int a = 5; //optimized a = 5

	//JSON MANAGER
	JsonManager json;

	//TWEET DATA
	bool isDisplay;
	vector <ofImage> tImages;
	vector <string> tNames;

	//ICON
	ofImage image;
	static const int RES_HUE = 4;
	static const int RES_SAT = 128;
	static const int RES_BRI = 128;
	ofImage icons[360/ RES_HUE][256 / RES_SAT][256 / RES_BRI];
	glm::vec3 avg[360/ RES_HUE][256 / RES_SAT][256 / RES_BRI];

	//SOUND
	ofSoundPlayer sound;

	//POST EFFECT
	ofFbo fbo;
	bool isShader;
	ofxPostGlitch glitch;
	void selectShader(int num);

	//CAPTURE
	int capture_no = 0;
	ofImage capture;
};