// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Distributed under the Myo SDK license agreement. See LICENSE.txt for details.

// Modified by Kevin Pantelakis in 2016 in a Hackaton.
#include "DataCollector.h"

DataCollector::DataCollector() : onArm(false), isUnlocked(false), roll_w(0), pitch_w(0), currentPose() { this->theMyo = nullptr; }

DataCollector::DataCollector(myo::Myo* theMyo) : onArm(false), isUnlocked(false), roll_w(0), pitch_w(0), currentPose() {
	this->theMyo = theMyo;
}

DataCollector::~DataCollector() {
	if (theMyo != nullptr) {
		//Locks the Myo when the collector is destroyed .
		theMyo->lock();
	}
}

void DataCollector::onUnpair(myo::Myo* myo, uint64_t timestamp) {
	//Myo connection lost, initial state returned.
	roll_w = 0;
	pitch_w = 0;
	onArm = false;
	isUnlocked = false;
}

void DataCollector::onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat) {
	using std::atan2;
	using std::asin;
	using std::sqrt;
	using std::max;
	using std::min;

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
	if (roll_w > -15 && roll_w < 15) {
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
	if (pitch_w > -15 && pitch_w < 15) {
		pitch_w = 0;
	}

	//Reverse speeds
	else if (pitch_w <= -15) {
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
}

void DataCollector::onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
	currentPose = pose;
	if (pose != myo::Pose::unknown && pose != myo::Pose::rest) {
		// Tell the Myo to stay unlocked until told otherwise.
		myo->unlock(myo::Myo::unlockHold);
		// Notify the Myo that the pose has resulted in an action. The Myo will vibrate.
		myo->notifyUserAction();
	}

}

void DataCollector::onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState) {
	onArm = true;
	whichArm = arm;
}

void DataCollector::onArmUnsync(myo::Myo* myo, uint64_t timestamp) {
	onArm = false;
}

void DataCollector::onUnlock(myo::Myo* myo, uint64_t timestamp) {
	isUnlocked = true;
}

void DataCollector::onLock(myo::Myo* myo, uint64_t timestamp) {
	isUnlocked = false;
}

void DataCollector::print() {
	std::string poseString = currentPose.toString();
	// Print out the orientation. Orientation data is always available, even if no arm is currently recognized.
	std::cout << "Direction: " << roll_w << " , " << "Vitesse: " << (pitch_w) << "[" << (isUnlocked ? "unlocked" : "locked  ") << "]" << "[" << poseString << "]" << "         \r";
}

myo::Pose DataCollector::getPose() {
	return currentPose;
}

ServerClient* DataCollector::getClient() { return nullptr; }