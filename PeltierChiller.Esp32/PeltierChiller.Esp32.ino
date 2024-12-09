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
#include "CommandService.h"
#include "CommandError.h"

const String SOFTWARE_VERSION = "0.1.0";

const uint8_t SD_CS = 5;

Communication::Abstractions::BaseError* configurationError;

uint16_t _communicationDelay;

Services::ChillerService * _chillerService;
 
Services::JsonService* _jsonService;

Communication::Services::CommunicationService* _communicationService;

Services::FileService* _fileService;

Services::ConfigurationService* _configurationService;

Services::PwmService* _pwmService;

Services::CommandService* _commandService;

void setup()  
{
	Wire.begin();

	_communicationService = new Communication::Services::SerialCommunicationService(460800);
	_communicationService->init();

	_commandService = new Services::CommandService();

	_fileService = new Services::FileService();
	_fileService->init(SD_CS);

	_jsonService = new Services::JsonService();

	_configurationService = new Services::ConfigurationService(_fileService, _jsonService);
	configurationError = _configurationService->readConfigurationFromSd();

	xTaskCreate(
		readCommunicationDataTask,
		"readCommunicationDataTask",
		4096,
		NULL,
		3,
		NULL
	);

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

	SemaphoreHandle_t mutex = xSemaphoreCreateMutex();

	xTaskCreate(
		manageChillerTask,
		"manageChillerTask",
		4096,
		mutex,
		3,
		NULL
	);

	xTaskCreate(
		handlePwmsTask,
		"handlePwmsTask",
		4096,
		mutex,
		3,
		NULL
	);

	xTaskCreate(
		sendCommunicationDataTask,
		"sendCommunicationDataTask",
		4096,
		mutex,
		3,
		NULL
	);
}

void loop() { }

void manageChillerTask(void* argument)
{
	SemaphoreHandle_t mutex = (SemaphoreHandle_t)argument;
	while (true)
	{
		xSemaphoreTake(mutex, portMAX_DELAY);
		(*_chillerService).manageChiller();
		xSemaphoreGive(mutex);
	}
}

void handlePwmsTask(void* argument)
{
	SemaphoreHandle_t mutex = (SemaphoreHandle_t)argument;
	while (true)
	{
		vTaskDelay(_pwmService->getUpdatePwmDelay() / portTICK_PERIOD_MS);
		xSemaphoreTake(mutex, portMAX_DELAY);
		_pwmService->handlePwms((*_chillerService).getChillerLoadPercentage());
		xSemaphoreGive(mutex);
	}
}

void sendCommunicationDataTask(void* argument)
{
	SemaphoreHandle_t mutex = (SemaphoreHandle_t)argument;
	while (true)
	{
		vTaskDelay(_communicationDelay / portTICK_PERIOD_MS);
		xSemaphoreTake(mutex, portMAX_DELAY);
		String temperatureResponse = (*_jsonService).serializeObject(
			Helpers::JsonHelper::convertToBaseSerializableObjectArray(_chillerService->getTemperatureService()->getTemperatureSensors()),
			Communication::Enums::ResponseType::temperatureSensors);
		_communicationService->sendData(temperatureResponse);

		String pwmsResponse = (*_jsonService).serializeObject(
			Helpers::JsonHelper::convertToBaseSerializableObjectArray(_pwmService->getPwmItems()),
			Communication::Enums::ResponseType::pwms);
		_communicationService->sendData(pwmsResponse);
		xSemaphoreGive(mutex);
	}
}

void readCommunicationDataTask(void* argument)
{
	while (true)
	{
		String request = _communicationService->readData();
		if (request.length() > 0)
		{
			Models::Abstractions::BaseCommand* command = _jsonService->deserializeCommand(request);
			if (command != nullptr)
			{
				_commandService->handleCommand(command);
			}
			else
			{
				Communication::Models::Errors::CommandError* commandError =
					new Communication::Models::Errors::CommandError(request);
				String errorCommandJson = (*_jsonService).serializeObject(commandError);
				_communicationService->sendData(errorCommandJson);

				errorCommandJson.clear();
				delete commandError;
			}
		}

		request.clear();
	}
}

void initGlobalConfiguration()
{
	_communicationDelay = _configurationService->getConfiguration()->getTimersConfiguration()->getCommunicationDelay();
}