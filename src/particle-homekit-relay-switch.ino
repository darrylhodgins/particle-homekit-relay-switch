#include "HKServer.h"
#include "HKLog.h"
#include "homekit-relay-switch.h"

SerialLogHandler logHandler;

HomekitRelaySwitchAccessory *acc = new HomekitRelaySwitchAccessory(D7, LOW);

HKServer *hkServer = NULL;

void progress(Progress_t progress) {
	hkLog.info("Homekit progress callback: %d",progress);
}

/**
 * Particle.function handler to restart the device
 */
int restart(String extra) {
	System.reset();
	return 0;
}

/**
 * Particle.function handler to reset Homekit persistor and restart
 */
int resetAll(String extra) {
	HKPersistor().resetAll();
	System.reset();
	return 0;
}

void setup() {
	randomSeed(Time.now());//we need to somehow init random seed, so device identity will be unique
	Serial.begin();
	
	hkServer = new HKServer(acc->getDeviceType(),"Homestation","523-12-643",progress);

	acc->initAccessorySet();

	hkServer->start();

	Particle.function("restart", restart);
	Particle.function("resetAll", resetAll);
}

void loop() {
	bool didAnything = false; //!hkServer->hasConnections();
	didAnything |= hkServer->handle(); //handle connections, did anything (i.e processed some requests etc.)
	didAnything |= acc->handle(); //handle accessory, did anything (i.e read some sensors)
	if(didAnything) {
		//hkLog.info("Free memory %lu",System.freeMemory());
	}
}
