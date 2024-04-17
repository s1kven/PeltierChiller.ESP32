/*
 Name:		PeltierChiller.ino
 Created:	3/7/2022 10:10:28 PM
 Author:	S1kven
*/

#pragma once

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <GyverBME280.h>
#include <HID.h>
#include <TFT.h>
#include <SPI.h>
#include <Wire.h>
#include <LinkedList.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "ChillerService.h"
#include "PowerButtonService.h"
#include "BaseSensor.h"
#include "DS18B20.h"
#include "NTC.h"
#include "Button.h"
#include "KeyValuePair.h"
#include "JsonService.h"
#include "JsonHelper.h"

#define cs 10
#define dc 9
#define rst 8

const uint8_t TSENSOR_PIN = 7;
const uint8_t CHILLERPSSIGNAL_PIN = A1;
const uint8_t CHILLERSIGNAL_PIN = A2;
const uint8_t PCV_PIN = A3;
const uint8_t POWERSIGNAL_PIN = 5;
const uint8_t POWERBUTTON_PIN = A5;
const uint8_t NTC_PIN = A0;

const float _targetCircuitTemperature = 20;

float vin = 0;
//uint32_t tftTimer = 0;

bool powerSignalFlag = false;
uint32_t tmr = 0;
uint32_t oldTmr = 0;

DeviceAddress room = { 0x28, 0xCD, 0x86, 0x56, 0xB5, 0x01, 0x3C, 0x1D };
DeviceAddress hotCircuit = { 0x28, 0xD6, 0x93, 0x81, 0xE3, 0xBC, 0x3C, 0xB6 };
DeviceAddress pcCase = { 0x28, 0x2B, 0x8E, 0x81,0xE3, 0xE4, 0x3C, 0x1F };
DeviceAddress coldCircuit = { 0x28, 0xA7, 0xC6, 0x14, 0x00, 0x00, 0x00, 0x4F };


Models::TemperatureSensors::BaseSensor* _tSensors[] =
{ 
	new Models::TemperatureSensors::DS18B20(&coldCircuit, Models::Enums::TemperatureSensorTarget::coldCircuit),
	new Models::TemperatureSensors::DS18B20(&room, Models::Enums::TemperatureSensorTarget::room),
	new Models::TemperatureSensors::DS18B20(&pcCase, Models::Enums::TemperatureSensorTarget::pcCase),
	new Models::TemperatureSensors::NTC(NTC_PIN, 10000, 3950, 10000,
		Models::Enums::TemperatureSensorTarget::room),
	new Models::TemperatureSensors::DS18B20(&hotCircuit, Models::Enums::TemperatureSensorTarget::hotCircuit),
	new Models::TemperatureSensors::BME280(0x76, Models::Enums::TemperatureSensorTarget::room)
};

Models::Button* powerButton = new Models::Button(POWERBUTTON_PIN, "POWER");

Services::ChillerService * _chillerService;

Services::JsonService* _jsonService;

//TFT TFTscreen = TFT(cs, dc, rst);

//char coldCircuitT[6];
//char hotCircuitT[6];
//char roomCircuitT[6];
//char pcCaseCircuitT[6];
//char pressureCH[6];
//char humidityCH[6];

void setup()  
{
	Wire.begin();

	Serial.begin(9600);

	_chillerService = new Services::ChillerService(TSENSOR_PIN, sizeof(_tSensors) / sizeof(int), _tSensors,
		_targetCircuitTemperature, Models::Enums::ChillerState::off);
	
	_jsonService = new Services::JsonService(2000);

	powerButton->setLastPressTime(0);

	pinMode(POWERBUTTON_PIN, INPUT_PULLUP);
	pinMode(CHILLERPSSIGNAL_PIN, OUTPUT);
	pinMode(CHILLERSIGNAL_PIN, OUTPUT);
	pinMode(POWERSIGNAL_PIN, OUTPUT);

	/*TFTscreen.begin();
	TFTscreen.setRotation(270);
	TFTscreen.background(255, 255, 255);
	TFTscreen.setTextSize(2);*/
}


void loop() 
{
	vin = float(analogRead(PCV_PIN));

	powerButton->setState(!digitalRead(powerButton->getSignalPin()));
	if (powerButton->getState() && !powerButton->getFlag())
	{
		powerButton->setFlag(true);
		powerButton->setLastPressTime(millis());
	}
	else if (!powerButton->getState() && powerButton->getFlag() && millis() - powerButton->getLastPressTime() > 20)
	{
		powerButton->setFlag(false);
		powerButton->setLastPressMillis(millis() - powerButton->getLastPressTime());
	}

	(*_chillerService).execute();

	selectChillerState();
	
	(*_jsonService).serializeAndSendToSerialPort(Helpers::JsonHelper::convertToBaseJsonModelArray(_tSensors, sizeof(_tSensors) / sizeof(int)));

	//tftOutput(); 
}

void selectChillerState()
{
	if (vin > 30 && (*_chillerService).GetChillerState() != Models::Enums::ChillerState::enabling)
	{
		(*_chillerService).SetChillerState(Models::Enums::ChillerState::temperatureMaintaining);
	}

	switch ((*_chillerService).GetChillerState())
	{
	case Models::Enums::ChillerState::off:
		if (powerButton->getLastPressMillis() > 0)
		{
			(*_chillerService).SetChillerState(Models::Enums::ChillerState::enabling);
		}
		else
		{
			digitalWrite(CHILLERPSSIGNAL_PIN, LOW);
			digitalWrite(CHILLERSIGNAL_PIN, LOW);
		}
		break;

	case Models::Enums::ChillerState::enabling:

		digitalWrite(CHILLERPSSIGNAL_PIN, HIGH);
		digitalWrite(CHILLERSIGNAL_PIN, HIGH);
		if (powerButton->getLastPressMillis() > 0 &&
			(*_chillerService).getTemperatureService()->getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::coldCircuit) <=
			(*_chillerService).getTargetTemperature())
		{
			digitalWrite(POWERSIGNAL_PIN, HIGH);
			if (tmr == oldTmr)
			{
				tmr = millis();
			}
			if (powerSignalFlag)
			{
				digitalWrite(POWERSIGNAL_PIN, LOW);
				tmr = millis();
				oldTmr = tmr;
				powerButton->setLastPressMillis(0);
				powerSignalFlag = false;
				(*_chillerService).SetChillerState(Models::Enums::ChillerState::temperatureMaintaining);
			}
			else if (millis() - powerButton->getLastPressMillis() >= tmr)
			{
				powerSignalFlag = true;
			}
		}
		break;

	case Models::Enums::ChillerState::temperatureMaintaining:

		digitalWrite(CHILLERPSSIGNAL_PIN, HIGH);
		digitalWrite(CHILLERSIGNAL_PIN, HIGH);
		if (vin < 30)
		{
			(*_chillerService).SetChillerState(Models::Enums::ChillerState::off);
		}
		/*if (powerButton->getLastPressMillis() != 0)
		{
			Serial.println("temperatureMaintaining high");
			digitalWrite(POWERSIGNAL_PIN, HIGH);
			(*_chillerService).SetChillerState(Models::Enums::ChillerState::disabling);
		}*/
		break;
	case Models::Enums::ChillerState::disabling:

		//Serial.println("disabling");
		//digitalWrite(CHILLERPSSIGNAL_PIN, HIGH);
		//digitalWrite(CHILLERSIGNAL_PIN, HIGH);
		if (!powerButton->getFlag() && millis() - powerButton->getLastTime() < powerButton->getLastPressMillis() &&
			powerButton->getLastPressMillis() != 0)
		{
			//Serial.println("disabling high");
			digitalWrite(POWERSIGNAL_PIN, HIGH);
		}
		else
		{
			//Serial.println("disabling low");
			digitalWrite(POWERSIGNAL_PIN, LOW);
			(*_chillerService).SetChillerState(Models::Enums::ChillerState::off);
		}
		break;

	default:
		break;
	}
}

//void tftOutput()
//{
//	if (millis() - tftTimer >= 5000)
//	{
//		tftTimer = millis();
//
//		String coldCT = String((*_chillerService).getTemperatureService()->
//			getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::coldCircuit), 4);
//		String hotCT = String((*_chillerService).getTemperatureService()->
//			getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::hotCircuit), 4);
//		String pcCaseCT = String((*_chillerService).getTemperatureService()->
//			getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::pcCase), 4);
//		String roomCT = String((*_chillerService).getTemperatureService()->
//			getTemperatureForSpecificTarget(Models::Enums::TemperatureSensorTarget::room), 4);
//		String pressure = String((*_chillerService).getTemperatureService()->
//			getPressureForSpecificTarget(Models::Enums::TemperatureSensorTarget::room), 4);
//		String humidity = String((*_chillerService).getTemperatureService()->
//			getHumidityForSpecificTarget(Models::Enums::TemperatureSensorTarget::room), 4);
//
//		coldCT.toCharArray(coldCircuitT, 6);
//		hotCT.toCharArray(hotCircuitT, 6);
//		roomCT.toCharArray(roomCircuitT, 6);
//		pcCaseCT.toCharArray(pcCaseCircuitT, 6);
//		pressure.toCharArray(pressureCH, 6);
//		humidity.toCharArray(humidityCH, 6);
//
//		TFTscreen.background(255, 255, 255);
//		TFTscreen.stroke(0, 0, 0);
//
//		TFTscreen.text((*_chillerService).getTemperatureService()->
//			getTemperatureSensorTargetName(Models::Enums::TemperatureSensorTarget::coldCircuit), 5, 5);
//		TFTscreen.text(coldCircuitT, 5, 25);
//
//		TFTscreen.text((*_chillerService).getTemperatureService()->
//			getTemperatureSensorTargetName(Models::Enums::TemperatureSensorTarget::hotCircuit), 5, 45);
//		TFTscreen.text(hotCircuitT, 5, 65);
//		TFTscreen.text(pressureCH, 65, 45);
//		TFTscreen.text(humidityCH, 65, 65);
//
//		TFTscreen.text((*_chillerService).getTemperatureService()->
//			getTemperatureSensorTargetName(Models::Enums::TemperatureSensorTarget::room), 5, 85);
//		TFTscreen.text(roomCircuitT, 5, 105);
//
//		TFTscreen.text((*_chillerService).getTemperatureService()->
//			getTemperatureSensorTargetName(Models::Enums::TemperatureSensorTarget::pcCase), 5, 125);
//		TFTscreen.text(pcCaseCircuitT, 5, 145);
//	}
//}
