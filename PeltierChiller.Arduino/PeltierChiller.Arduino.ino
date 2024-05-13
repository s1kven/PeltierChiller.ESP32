/*
 Name:		PeltierChiller.ino
 Created:	3/7/2022 10:10:28 PM
 Author:	S1kven
*/

#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <GyverBME280.h>
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
#include "BaseDeserializableObject.h"
#include "JsonService.h"
#include "JsonHelper.h"
#include "CommunicationService.h"
#include "SerialCommunicationService.h"
#include "FileService.h"
#include "ConfigurationService.h"

//      arduino      ->      esp32
//        SCL                 22
//        SDA                 21
//        CS(10)              15
//        DC(9)               4
//        RST(8)              17
//    TSENSOR_PIN(7)          16
// CHILLERPSSIGNAL_PIN(A1)    32
//   CHILLERSIGNAL_PIN(A2)    33
//       PCV_PIN(A3)          25
//    POWERBUTTON_PIN(A5)     26
//        NTC_PIN(A0)         34
//    POWERSIGNAL_PIN(5)      27
//        SCK(13)             14
//        MOSI(11)            13

const uint8_t TSENSOR_PIN = 16;
const uint8_t CHILLERPSSIGNAL_PIN = 32;
const uint8_t CHILLERSIGNAL_PIN = 33;
const uint8_t PCV_PIN = 25;
const uint8_t POWERSIGNAL_PIN = 27;
const uint8_t POWERBUTTON_PIN = 26;
const uint8_t NTC_PIN = 34;
const uint8_t SD_CS = 5;

float vin = 0;

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
		Models::Enums::TemperatureSensorTarget::room, 25, 3.3, 12),
	new Models::TemperatureSensors::DS18B20(&hotCircuit, Models::Enums::TemperatureSensorTarget::hotCircuit),
	new Models::TemperatureSensors::BME280(0x76, Models::Enums::TemperatureSensorTarget::room)
};

Models::Button* powerButton;

Services::ChillerService * _chillerService;
 
Services::JsonService* _jsonService;

Communication::Services::CommunicationService* _communicationService;

Services::FileService* _fileService;

Services::ConfigurationService* _configurationService;

void setup()  
{
	Wire.begin();

	_communicationService = new Communication::Services::SerialCommunicationService(115200);
	_communicationService->init();

	powerButton = new Models::Button(POWERBUTTON_PIN, "POWER");
	powerButton->setLastPressTime(0);

	pinMode(POWERBUTTON_PIN, INPUT_PULLUP);
	pinMode(CHILLERPSSIGNAL_PIN, OUTPUT);
	pinMode(CHILLERSIGNAL_PIN, OUTPUT);
	pinMode(POWERSIGNAL_PIN, OUTPUT);

	_fileService = new Services::FileService();
	_fileService->init(SD_CS);

	_jsonService = new Services::JsonService(2000);

	_configurationService = new Services::ConfigurationService(_fileService, _jsonService);

	_chillerService = new Services::ChillerService(TSENSOR_PIN, sizeof(_tSensors) / sizeof(int), _tSensors,
		_configurationService->getConfiguration(), Models::Enums::ChillerState::off);
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
	
	(*_jsonService).serializeAndSendToSerialPort(Helpers::JsonHelper::convertToBaseJsonModelArray(_tSensors, sizeof(_tSensors) / sizeof(int)),
		Communication::Enums::ResponseType::temperatureSensors);

	//tftOutput(); 
}

void selectChillerState()
{
	if (vin > 250 && (*_chillerService).GetChillerState() != Models::Enums::ChillerState::enabling)
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
