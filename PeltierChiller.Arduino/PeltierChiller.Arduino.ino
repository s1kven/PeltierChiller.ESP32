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

const uint8_t SD_CS = 5;

uint8_t PCV_PIN;

float vin = 0;
uint32_t _communicationSendTimer = 0;

uint16_t _communicationDelay;

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

	_fileService = new Services::FileService();
	_fileService->init(SD_CS);

	_jsonService = new Services::JsonService();

	_configurationService = new Services::ConfigurationService(_fileService, _jsonService);
	_configurationService->readConfigurationFromSd();
	initGlobalConfiguration();

	_chillerService = new Services::ChillerService(_configurationService->getConfiguration());
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
			Helpers::JsonHelper::convertToBaseSerializableObjectArray(_chillerService->getTemperatureService()->getTemperatureSensors()),
			Communication::Enums::ResponseType::temperatureSensors);
		_communicationService->sendData(response);
	}
}

void initGlobalConfiguration()
{
	_communicationDelay = _configurationService->getConfiguration()->getTimersConfiguration()->getCommunicationDelay();
	PCV_PIN = _configurationService->getConfiguration()->getPinsConfiguration()->getPcVoltage();
}
