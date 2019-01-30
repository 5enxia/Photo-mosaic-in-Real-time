#include "ofApp.h"

void ofApp::setup() {
	//GENERAL
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	ofBackground(0);

	//JSON
	string file = "twitter.json";
	json.load(file);
	json.parse();

	//WEB CAMERA
	video.initGrabber(160, 120);
	isCamStream = true;
	isCamDraw = false;

	//POST EFFECT
	fbo.allocate(ofGetWidth(), ofGetHeight());
	isShader = false;
	glitch.setup(&fbo);

	//ICON
	size = 12;
	a = 1;

	//SOUND
	sound.load("se.mp3");

	//TWEET DISPLAY
	isDisplay = true;
}

void ofApp::update() {
	//JSON
	json.update();

	//ICON
	try
	{
		image.load(json.tweet.back().getUrl());
	}
	catch (const std::exception&)
	{
		cout << "Wrong format was loaded!" << endl;
		json.update();
		image.load(json.tweet.back().getUrl());
	}

	int num = 0;
	int sumH = 0;
	int sumS = 0;
	int sumB = 0;
	for (int i = 0; i < image.getWidth(); i++)
	{
		for (int j = 0; j < image.getHeight(); j++)
		{
			int h = image.getColor(i, j).getHue();
			int s = image.getColor(i, j).getSaturation();
			int b = image.getColor(i, j).getBrightness();
			if (h > 359) {
				h = 359;
			}
			num++;
			sumH += h;
			sumS += s;
			sumB += b;
		}
	}
	icons[sumH / num / RES_HUE][sumS / num / RES_SAT][sumB / num / RES_BRI] = image;
	avg[sumH / num / RES_HUE][sumS / num / RES_SAT][sumB / num / RES_BRI] = glm::vec3(sumH, sumS, sumB) / num;

	//RGB CAMERA
	if (isCamStream)
	{
		video.update();
		if (video.isFrameNew())
		{
			video2pix = video.getPixels();
		}
	}

	//FBO BEGIN
	fbo.begin();
	ofClear(0);

	//DRAW TO FBO
	for (int i = 0; i < video2pix.getWidth(); i += a)
	{
		for (int j = 0; j < video2pix.getHeight(); j += a)
		{
			int h = video2pix.getColor(i, j).getHue();
			int s = video2pix.getColor(i, j).getSaturation();
			int b = video2pix.getColor(i, j).getBrightness();
			if (h > 359)
			{
				h = 359;
			}
			bool flagH = h != 0 && avg[h / RES_HUE][s / RES_SAT][b / RES_BRI].x != 0;
			bool flagS = s != 0 && avg[h / RES_HUE][s / RES_SAT][b / RES_BRI].y != 0;
			bool flagB = b != 0 && avg[h / RES_HUE][s / RES_SAT][b / RES_BRI].z != 0;
			bool flag = flagH || flagS || flagB;
			if (flag) {
				icons[h / RES_HUE][s / RES_SAT][b / RES_BRI].draw(i * size / a, j * size / a, size, size);
			}
			else {
				if (isCamDraw)
				{
					ofColor hsb = ofColor::fromHsb(h, s, b);
					ofSetColor(hsb);
					ofDrawRectangle(i * size / a, j * size / a, size, size);
				}
			}
		}
	}

	//FBO END
	fbo.end();

	//TWEET DATA
	if (json.isUpdating())
	{
		sound.stop();
		sound.play();
		if (tImages.size() < 10) {
			tImages.push_back(image);
			tNames.push_back(json.tweet.back().getName());
		}
		else {
			tImages.erase(tImages.begin());
			tImages.push_back(image);
			tNames.erase(tNames.begin());
			tNames.push_back(json.tweet.back().getName());
		}
		//POST EFFECT
		if (isShader) {
			int channel = ofRandom(4);
			selectShader(channel);
		}
	}
}

void ofApp::draw() {
	//POST EFFECT
	if (isShader)
	{
		glitch.generateFx();
	}
	fbo.draw(0, 12);

	//TWEET DATA
	if (isDisplay)
	{
		int s = 96;
		ofPushMatrix();
		ofTranslate(1536 + 48, 48 + 12);
		for (int i = tImages.size() - 1; i >= 0; i--)
		{
			tImages[i].draw(0, i * s, s, s);
			ofDrawBitmapString("@" + tNames[i], s, i * s + 10);
		}
		ofPopMatrix();
	}
}

void ofApp::keyPressed(int key)
{
	//WEB CAM
	if (key == ' ')
	{
		isCamStream ^= 1;
	}

	if (key == 'q')
	{
		isCamStream = false;
		video.close();
	}

	if (key == 'c') {
		isCamDraw ^= 1;
	}

	if (key == 'h') {
		isDisplay ^= 1;
	}

	if (key == 's') {
		isShader ^= 1;
	}

	//ICON SIZE
	if (key == OF_KEY_LEFT) {
		if (size < 48) {
			size = 48;
			a = 5;
		}
	}
	if (key == OF_KEY_RIGHT) {
		if (size > 12) {
			size = 12;
			a = 1;
		}
	}

	//CAPTURE
	if (key == OF_KEY_RETURN)
	{
		capture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		capture.save("capture/capture_ "+ ofToString(capture_no) +".png");
		cout << "CAPTURED" << endl;
		capture_no++;
	}
}

//POST EFFECT
void ofApp::selectShader(int channel)
{
	glitch.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
	glitch.setFx(OFXPOSTGLITCH_TWIST, false);
	glitch.setFx(OFXPOSTGLITCH_SWELL, false);
	glitch.setFx(OFXPOSTGLITCH_INVERT, false);

	if (channel == 0) {
		glitch.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
	}
	else if (channel == 1) {
		glitch.setFx(OFXPOSTGLITCH_TWIST, true);
	}
	else if (channel == 2) {
		glitch.setFx(OFXPOSTGLITCH_SWELL, true);
	}
	else {
		glitch.setFx(OFXPOSTGLITCH_INVERT, true);
	}
}

