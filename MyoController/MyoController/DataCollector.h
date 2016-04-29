// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Distributed under the Myo SDK license agreement.

// Modified by Kevin Pantelakis in 2016 in a Hackaton.
#pragma once
#include "myo.hpp"
#include <math.h>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <SDL2\SDL.h>
#include <SDL2\SDL_Net.h>
#include "SocketException.h"
#include "Info.h"
#include "ServerClient.h"

class DataCollector : public myo::DeviceListener
{
protected:
	// These values are set by onArmSync() and onArmUnsync().
	bool onArm;
	bool isUnlocked;
	myo::Arm whichArm;
	// These values are set by onOrientationData() and onPose().
	int roll_w, pitch_w;
	myo::Pose currentPose;
	myo::Myo* theMyo;
	ServerClient* client;

public:
	DataCollector();
	DataCollector(myo::Myo* theMyo);
	~DataCollector();
	void onUnpair(myo::Myo* myo, uint64_t timestamp);
	virtual void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat);
	void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose);
	void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState);
	void onArmUnsync(myo::Myo* myo, uint64_t timestamp);
	void onUnlock(myo::Myo* myo, uint64_t timestamp);
	void onLock(myo::Myo* myo, uint64_t timestamp);
	void print();
	myo::Pose getPose();
	ServerClient* getClient();
};

