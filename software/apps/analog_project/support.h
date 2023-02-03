#include "nrf.h"

#include "stdbool.h"
#include "gpio.h"


typedef struct accellerometers{
	float X;
	float Y;
	float Z;

};

void setDevices();


void deviceLoop();


void printTrio();
bool getInput();

void Record();