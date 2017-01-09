/**
 * Arduino Serial mock
 */
#ifndef SMARTCAR_H
#define SMARTCAR_H

// The various pin definitions and standard arduino utilities
#include "pins_arduino.h"

// Fakes for the different motor types and helper methods
#include "Motors.h"

// The mock for the Car class
#include "Car.h"
#include "UltrasonicSensors.h"
#include "InfraredSensors.h"
#include "HeadingSensors.h"
#include "Odometer.h"

#endif // SMARTCAR_H
