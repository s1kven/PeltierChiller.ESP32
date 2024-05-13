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
uint32_t _communicationSendTimer = 0;

const uint16_t _communicationDelay = 2000;

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

	pinMode(POWERBUTTON_PIN, INPUT_PULLUP);
	pinMode(CHILLERPSSIGNAL_PIN, OUTPUT);
	pinMode(CHILLERSIGNAL_PIN, OUTPUT);
	pinMode(POWERSIGNAL_PIN, OUTPUT);

	_fileService = new Services::FileService();
	_fileService->init(SD_CS);

	_jsonService = new Services::JsonService();

	_configurationService = new Services::ConfigurationService(_fileService, _jsonService);

	_chillerService = new Services::ChillerService(TSENSOR_PIN, sizeof(_tSensors) / sizeof(int), _tSensors,
		_configurationService->getConfiguration(), POWERBUTTON_PIN, CHILLERPSSIGNAL_PIN, CHILLERSIGNAL_PIN, POWERSIGNAL_PIN);
}


void loop() 
{
	handlePcVoltage();

	(*_chillerService).manageChiller(vin);

	sendDataByTimer();
}

void handlePcVoltage()
{
	vin = float(analogRead(PCV_PIN));
}

void sendDataByTimer()
{
	if (millis() - _communicationSendTimer >= _communicationDelay)
	{
		_communicationSendTimer = millis();

		String response = (*_jsonService).serializeObject(
			Helpers::JsonHelper::convertToBaseSerializableObjectArray(_tSensors, sizeof(_tSensors) / sizeof(int)),
			Communication::Enums::ResponseType::temperatureSensors);
		_communicationService->sendData(response);
	}
}
