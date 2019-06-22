#include "homekit-relay-switch.h"

#include "HKConnection.h"
#include "HKLog.h"

#include <set>

std::string HomekitRelaySwitchAccessory::getPower (HKConnection *sender) {
	return on ? "true" : "false";
}

void HomekitRelaySwitchAccessory::setPower (bool oldValue, bool newValue, HKConnection *sender) {
	on = newValue;
}

void HomekitRelaySwitchAccessory::identify(bool oldValue, bool newValue, HKConnection *sender) {
	Serial.printf("Start Identify\n");
}


bool HomekitRelaySwitchAccessory::handle() {
	digitalWrite(pin, on ? HIGH : LOW);
	return false;
}

void HomekitRelaySwitchAccessory::initAccessorySet() {
	pinMode(pin, OUTPUT);
	Accessory *switchAcc = new Accessory();

	//Add Accessory
	AccessorySet *accSet = &AccessorySet::getInstance();
	addInfoServiceToAccessory(switchAcc, "Switch", "ljezny", "Switch", "1","1.0.0", std::bind(&HomekitRelaySwitchAccessory::identify, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3));
	accSet->addAccessory(switchAcc);

	Service *switchService = new Service(serviceType_switch);
	switchAcc->addService(switchService);

	stringCharacteristics *switchServiceName = new stringCharacteristics(charType_serviceName, premission_read, 0);
	switchServiceName->characteristics::setValue("Relay Switch");
	switchAcc->addCharacteristics(switchService, switchServiceName);

	boolCharacteristics *powerState = new boolCharacteristics(charType_on, premission_read|premission_write|premission_notify);
	powerState->perUserQuery = std::bind(&HomekitRelaySwitchAccessory::getPower, this, std::placeholders::_1);
	powerState->valueChangeFunctionCall = std::bind(&HomekitRelaySwitchAccessory::setPower, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
	switchAcc->addCharacteristics(switchService, powerState);
}
