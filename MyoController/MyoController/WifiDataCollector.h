#pragma once
#include "DataCollector.h"


class WifiDataCollector : public DataCollector
{

private:

public:
	WifiDataCollector(myo::Myo* theMyo);
	~WifiDataCollector();
	void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat);
	ServerClient* getClient();

};

