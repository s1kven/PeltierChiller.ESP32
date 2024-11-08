/*
 Name:		PeltierChiller.ino
 Created:	3/7/2022 10:10:28 PM
 Author:	S1kven
*/

#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "Test.h"
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
#include "PwmService.h"

const String SOFTWARE_VERSION = "0.1.0";

const uint8_t SD_CS = 5;

uint32_t _communicationSendTimer = 0;

Communication::Abstractions::BaseError* configurationError;

uint16_t _communicationDelay;

Services::ChillerService * _chillerService;
 
Services::JsonService* _jsonService;

Communication::Services::CommunicationService* _communicationService;

Services::FileService* _fileService;

Services::ConfigurationService* _configurationService;

Services::PwmService* _pwmService;

void setup()  
{
	Wire.begin();

	_communicationService = new Communication::Services::SerialCommunicationService(115200);
	_communicationService->init();

	_fileService = new Services::FileService();
	_fileService->init(SD_CS);

	_jsonService = new Services::JsonService();

	_configurationService = new Services::ConfigurationService(_fileService, _jsonService);
	configurationError = _configurationService->readConfigurationFromSd();
	if (*configurationError)
	{
		String errorConfigurationRead = (*_jsonService).serializeObject(configurationError);

		_communicationService->sendData(errorConfigurationRead);
		return;
	}

	initGlobalConfiguration();

	_chillerService = new Services::ChillerService(_configurationService->getConfiguration());

	_pwmService = new Services::PwmService(
		_configurationService->getConfiguration()->getTimersConfiguration()->getUpdatePwmDelay(), 
		_configurationService->getConfiguration()->getPwmsConfiguration(),
		_chillerService->getTemperatureService());
}


void loop() 
{
	if (*configurationError)
	{
		return;
	}

	(*_chillerService).manageChiller();
	_pwmService->handlePwms();

	sendDataByTimer();
}

void sendDataByTimer()
{
	if (millis() - _communicationSendTimer >= _communicationDelay)
	{
		_communicationSendTimer = millis();

		String temperatureResponse = (*_jsonService).serializeObject(
			Helpers::JsonHelper::convertToBaseSerializableObjectArray(_chillerService->getTemperatureService()->getTemperatureSensors()),
			Communication::Enums::ResponseType::temperatureSensors);
		_communicationService->sendData(temperatureResponse);

		String pwmsResponse = (*_jsonService).serializeObject(
			Helpers::JsonHelper::convertToBaseSerializableObjectArray(_pwmService->getPwmItems()),
			Communication::Enums::ResponseType::pwms);
		_communicationService->sendData(pwmsResponse);
	}
}

void initGlobalConfiguration()
{
	_communicationDelay = _configurationService->getConfiguration()->getTimersConfiguration()->getCommunicationDelay();
}