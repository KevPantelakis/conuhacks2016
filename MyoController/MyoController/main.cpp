// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Distributed under the Myo SDK license agreement. See LICENSE.txt for details.


// Adapted by Kevin Pantelakis in 2016.
#include "DataCollector.h"
#include "WifiDataCollector.h"

int main(int argc, char** argv)
{
	SDLNet_Init();
	// We catch any exceptions that might occur below -- see the catch statement for more details.

		// First, we create a Hub with our application identifier. Be sure not to use the com.example namespace when
		// publishing your application. The Hub provides access to one or more Myos.
		myo::Hub hub("com.example.myo-controller");

		std::cout << "Attempting to find a Myo..." << std::endl;

		// Next, we attempt to find a Myo to use. If a Myo is already paired in Myo Connect, this will return that Myo
		// immediately.
		// waitForMyo() takes a timeout value in milliseconds. In this case we will try to find a Myo for 10 seconds, and
		// if that fails, the function will return a null pointer.
		myo::Myo* myo = hub.waitForMyo(10000);

		// If waitForMyo() returned a null pointer, we failed to find a Myo, so exit with an error message.
		if (!myo) {
			throw std::runtime_error("Unable to find a Myo!");
		}

		// We've found a Myo.
		std::cout << "Connected to a Myo armband!" << std::endl << std::endl;

		// Next we construct an instance of our DeviceListener, so that we can register it with the Hub.
		WifiDataCollector collector(myo);

		// Hub::addListener() takes the address of any object whose class inherits from DeviceListener, and will cause
		// Hub::run() to send events to all registered device listeners.
		hub.addListener(&collector);

		//Unlock the Myo, vribrate and wait for user action to start controling the robot
		std::cout << "Wave out to start!" << std::endl << std::endl;
		myo->unlock(myo::Myo::unlockHold);
		myo->notifyUserAction();
		for (;;) {
			hub.run(1);
			if (collector.getPose() == myo::Pose::waveOut)
				break;
		}
		std::cout << "DoubleTap to start exit sequence" << std::endl;
		if (collector.getClient() == nullptr) {
			//The main loop.
			for (;;) {
				// In each iteration of our main loop, we run the Myo event loop for a set number of milliseconds.
				// In this case, we wish to update our display 20 times a second, so we run for 1000/20 milliseconds.
				hub.run(1000 / 20);
				// After processing events, we call the print() member function we defined above to print out the values we've
				// obtained from any events that have occurred.
				collector.print();
				if (collector.getPose() == myo::Pose::doubleTap) {
					//confirm exit
					std::cout << "Wave out to exit or Wave in to continue              " << std::endl << std::endl;
					for (;;) {

						hub.run(1);
						if (collector.getPose() == myo::Pose::waveOut)
							return 0;
						else if (collector.getPose() == myo::Pose::waveIn) break;
					}
				}
			}
		}
		else {
			while (!collector.getClient()->getShutdownStatus()) {
				collector.getClient()->checkForIncomingMessages();
				// In each iteration of our main loop, we run the Myo event loop for a set number of milliseconds.
				// In this case, we wish to update our display 20 times a second, so we run for 1000/20 milliseconds.
				hub.run(1000 / 20);
				// After processing events, we call the print() member function we defined above to print out the values we've
				// obtained from any events that have occurred.
				collector.print();
				if (collector.getPose() == myo::Pose::doubleTap) {
					//confirm exit
					std::cout << "Wave out to exit or Wave in to continue              " << std::endl << std::endl;
					for (;;) {
						hub.run(1);
						if (collector.getPose() == myo::Pose::waveOut)
							return 0;
						else if (collector.getPose() == myo::Pose::waveIn) break;
					}
				}
			}
		}
		// If a standard exception occurred, we print out its message and exit.


	return 0;
}
