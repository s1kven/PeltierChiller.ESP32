/*
 Name:		PeltierChiller.ino
 Created:	3/7/2022 10:10:28 PM
 Author:	Admin
*/

// the setup function runs once when you press reset or power the board
#pragma once

#include <LinkedList.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "ChillerService.h"

const uint8_t PELTIER_PIN = 10;
const uint8_t TSENSOR_PIN = 2;

const float _targetCircuitTemperature = 27;

Services::ChillerService * _chillerService;

void setup() 
{

	LinkedList<int8_t> * _peltierPins = new LinkedList<int8_t>();
	LinkedList<int8_t> * _temperatureSensorsPin = new LinkedList<int8_t>();

	(*_peltierPins).add(PELTIER_PIN);
	(*_temperatureSensorsPin).add(TSENSOR_PIN);

	for (uint8_t index = 0; index < (*_peltierPins).size(); index++)
	{
		pinMode((*_peltierPins).get(index), OUTPUT);
	}

	_chillerService = new Services::ChillerService(*_peltierPins, *_temperatureSensorsPin, _targetCircuitTemperature);

	Serial.begin(9600);
}

// the loop function runs over and over again until power down or reset
void loop() 
{
	(*_chillerService).handleChillerState();
}
