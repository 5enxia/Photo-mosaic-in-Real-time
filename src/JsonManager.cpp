#include "JsonManager.h"

//LOAD JSON FILE
void JsonManager::load(string file) {
	this->file = file;
	bool success = json.open(file);
	if (success) {
		cout << "Successfully loaded json" << endl;
		id = json["id"].asInt64();
	}
	else {
		cout << "Failed to load json" << endl;
	}
}

//PARASE JSON FILE
void JsonManager::parse()
{
	string screen_name = json["user"]["screen_name"].asString();
	string img_url = json["user"]["profile_image_url"].asString();

	tweet.push_back(Tweet(id, screen_name, img_url));
}

//UPDATE JSON FILE
void JsonManager::update() {
	if (tweet.size() > DATA_MAX_NUM) {
		tweet.erase(tweet.begin());
		cout << "DELETE HEAD DATA" << endl;
	}

	bool success = json.open(file);

	if (success) {
		if (checkUpdate()) {
			parse();
		}
	}
	else
	{
		cout << "Failed to parse JSON" << endl;
	}
}

//CHECK UPADATING STATE
bool JsonManager::checkUpdate() {
	if (json["id"].asInt64() != id)
	{
		id = json["id"].asInt64();
		flag = true;
		return flag;
	}
	else {
		flag = false;
		return flag;
	}
}

//GET UPDATING STATE
bool JsonManager::isUpdating() {
	return flag;
}