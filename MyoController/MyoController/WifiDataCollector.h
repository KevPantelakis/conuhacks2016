#pragma once
#include "DataCollector.h"
/**/
#include <SDL2\SDL.h>
#include <SDL2\SDL_Net.h>
#include "SocketException.h"
#include "Info.h"
#include "ServerClient.h"

class WifiDataCollector : public DataCollector
{

private:
	ServerClient* client;
public:
	WifiDataCollector(myo::Myo* theMyo);
	~WifiDataCollector();
	void WifiDataCollector::onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat);
	ServerClient* getClient();

};

