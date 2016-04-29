#include "WifiDataCollector.h"

void MessageReaction(ServerInfo* info) {}

WifiDataCollector::WifiDataCollector(myo::Myo* theMyo) : DataCollector(theMyo){
	client = new ServerClient("104.236.69.2", 8889, 512, MessageReaction,"client");
	client->connectToServer();
}

ServerClient* WifiDataCollector::getClient() { return client; }

WifiDataCollector::~WifiDataCollector(){
	ClientInfo* clientinfo = new ClientInfo();
	clientinfo->ID = 0;
	clientinfo->message = new std::string("message");
	clientinfo->name = "Client";
	clientinfo->servo = 90;
	clientinfo->speed = 0;
	client->sendToServer(clientinfo);
	delete client;
}

void WifiDataCollector::onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat) {
	
	using std::atan2;
	using std::asin;
	using std::sqrt;
	using std::max;
	using std::min;
	ClientInfo* clientinfo = new ClientInfo();
	clientinfo->ID = 0;
	clientinfo->message = new std::string("message");
	clientinfo->name = "Client";

	// Calculate Euler angles (roll and pitch) from the unit quaternion.
	float roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
		1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));

	float pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));

	//Transforms the radians to degrees angles. 
	roll_w = (int)(roll * 180 / M_PI);
	pitch_w = (int)(pitch * 180 / M_PI);


	//To use in the robot servo motor, roll_w mut be between 45 and 135 degrees

	//Gives an interval of 30 degrees to stabilize at 0 degrees.
	//The 0 degree on the servo motor is at 90 degrees.
	if (roll_w >= -15 && roll_w <= 15) {
		roll_w = 90;
	}

	else if (roll_w < -15) {
		//roll_w min value is 45.
		if (roll_w < -60)
			roll_w = 45;
		else
			roll_w += 105;
	}
	else if (roll_w > 15) {
		//roll_w max value is 135
		if (roll_w > 60)
			roll_w = 135;
		else
			roll_w += 75;
	}

	//The robot has 6 diffirent speeds 

	//Gives an interval of 30 degrees to stabilize at 0 degrees.
	// Parking state
	if (pitch_w >= -15 && pitch_w <= 15) {
		pitch_w = 0;
	}

	//Reverse speeds
	else if (pitch_w < -15) {
		if (pitch_w >= -35) pitch_w = 4;
		else if (pitch_w >= -55) pitch_w = 5;
		else pitch_w = 6;
	}
	//Foward speeds
	else if (pitch_w > 15) {
		if (pitch_w <= 35) pitch_w = 1;
		else if (pitch_w <= 55) pitch_w = 2;
		else pitch_w = 3;
	}

	clientinfo->servo = roll_w;
	clientinfo->speed = pitch_w;
	client->sendToServer(clientinfo);

	delete clientinfo;


}