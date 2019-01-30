#include "ofMain.h"
class Tweet {
public:

	Tweet::Tweet(int64_t &id, string &screen_name, string & icon_url)
	{
		this->id = id;
		this->screen_name = screen_name;
		this->icon_url = icon_url;
	}

	//GET TWEET ID
	int64_t getId() {
		return this->id;
	}

	//GET SCREEN NAME
	string getName() {
		return this->screen_name;
	}

	//GET ICON IMAGE URL
	string getUrl() {
		return this->icon_url;
	}

private:
	int64_t id;
	string screen_name;
	string icon_url;
};