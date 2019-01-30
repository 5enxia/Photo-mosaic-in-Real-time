#pragma once
#include "ofMain.h"
#include "ofxJSON.h"
#include "Tweet.h"

//MAX NUMBER OF STORED TWEET
#define DATA_MAX_NUM 5

class JsonManager 
{
public:
	//ofxJSON
	ofxJSONElement json;

	//LOAD JOSN FILE
	string file;
	void load(string file);

	//PARSE JSON FILE
	int64_t id;
	void parse();

	//UPDATE JSON FILE
	void update();
	bool checkUpdate();
	bool isUpdating();
	
	//STORE TWEET
	vector<Tweet> tweet;

private:
	bool flag;
};