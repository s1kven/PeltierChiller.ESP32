/*
 Name:		PeltierChiller.ino
 Created:	3/7/2022 10:10:28 PM
 Author:	Admin
*/

// the setup function runs once when you press reset or power the board
#pragma once

#include <Wire.h>
#include <LinkedList.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "ChillerService.h"
#include "TemperatureSensor.h"

const uint8_t TSENSOR_PIN = 7;

const float _targetCircuitTemperature = 20;

DeviceAddress hotCircuit = { 0x28, 0xCD, 0x86, 0x56, 0xB5, 0x01, 0x3C, 0x1D };
DeviceAddress coldCircuit = { 0x28, 0xA7, 0xC6, 0x14, 0x00, 0x00, 0x00, 0x4F };

Models::TemperatureSensor* _tSensors[] =
{
	new Models::TemperatureSensor(&hotCircuit, Models::Enums::TemperatureSensorTarget::hotCircuit),
	new Models::TemperatureSensor(&coldCircuit, Models::Enums::TemperatureSensorTarget::coldCircuit)
};

Services::ChillerService * _chillerService;

void setup()  
{
	Wire.begin();

	_chillerService = new Services::ChillerService(TSENSOR_PIN, _tSensors, _targetCircuitTemperature);

	Serial.begin(9600);
}

// the loop function runs over and over again until power down or reset
void loop() 
{
	(*_chillerService).handleChillerState();
}
