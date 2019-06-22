// Example usage for homekit-relay-switch library by Darryl Hodgins <darrylh@darrylh.ca>.

#include "homekit-relay-switch.h"

// Initialize objects from the lib
Homekitrelayswitch homekitrelayswitch;

void setup() {
    // Call functions on initialized library objects that require hardware
    homekitrelayswitch.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    homekitrelayswitch.process();
}
