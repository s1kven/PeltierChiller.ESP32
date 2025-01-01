
/*
 Name:		PeltierChiller.ino
 Created:	3/7/2022 10:10:28 PM
 Author:	S1kven
*/

#pragma once
#ifndef _ENTRYPOINT_
#define _ENTRYPOINT_ 
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
#include "RequestService.h"
#include "UpdateConfigurationCommand.h"
#include "GetTemperatureSensorsData.h"
#include "GetPwmItemsData.h"

const String SOFTWARE_VERSION = "0.1.0";

const uint8_t SD_CS = 5;

uint16_t _communicationDelay;

Services::TemperatureService* _temperatureService;

Services::ChillerService* _chillerService;
 
Services::JsonService* _jsonService;

Communication::Services::CommunicationService* _communicationService;

Services::FileService* _fileService;

Services::ConfigurationService* _configurationService;

Services::PwmService* _pwmService;

Services::RequestService* _requestService;

void setup()  
{
	_temperatureService = nullptr;
	_chillerService = nullptr;
	_pwmService = nullptr;

	Wire.begin();

	_communicationService = new Communication::Services::SerialCommunicationService(460800);
	_communicationService->init();

	_requestService = new Services::RequestService();

	_fileService = new Services::FileService();
	_fileService->init(SD_CS);

	_jsonService = new Services::JsonService();

	_configurationService = new Services::ConfigurationService();
	Communication::Models::Responses::Response* configurationResponse = _configurationService->readConfigurationFromSd();

	SemaphoreHandle_t communicationMutex = xSemaphoreCreateMutex();
	xTaskCreate(
		readCommunicationDataTask,
		"readCommunicationDataTask",
		4096,
		communicationMutex,
		3,
		NULL
	);

	if (!configurationResponse->getSuccess())
	{
		_communicationService->sendResponse(configurationResponse);
		return;
	}

	_configurationService->initConfiguration();

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
		communicationMutex,
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
		vTaskDelay(10 / portTICK_PERIOD_MS);
		if (!_configurationService->isChangeConfiguration())
		{
			xSemaphoreTake(mutex, portMAX_DELAY);
			_chillerService->manageChiller();
			xSemaphoreGive(mutex);
		}
	}
}

void handlePwmsTask(void* argument)
{
	SemaphoreHandle_t mutex = (SemaphoreHandle_t)argument;
	while (true)
	{
		vTaskDelay(_pwmService->getUpdatePwmDelay() / portTICK_PERIOD_MS);
		if (!_configurationService->isChangeConfiguration())
		{
			xSemaphoreTake(mutex, portMAX_DELAY);
			_pwmService->handlePwms(_chillerService->getChillerLoadPercentage());
			xSemaphoreGive(mutex);
		}
	}
}

void sendCommunicationDataTask(void* argument)
{
	SemaphoreHandle_t mutex = (SemaphoreHandle_t)argument;
	while (true)
	{
		vTaskDelay(_communicationDelay / portTICK_PERIOD_MS);
		if (!_configurationService->isChangeConfiguration())
		{
			xSemaphoreTake(mutex, portMAX_DELAY);
			Communication::Models::Responses::Response* temperatureSensorsResponse =
				new Communication::Models::Responses::Response(Communication::Enums::ResponseType::temperatureSensors,
					new Communication::Models::Responses::ResponsesData::GetTemperatureSensorsData(
						_temperatureService->getTemperatureSensors()));
			_communicationService->sendResponse(temperatureSensorsResponse);

			Communication::Models::Responses::Response* pwmItemsResponse =
				new Communication::Models::Responses::Response(Communication::Enums::ResponseType::pwms,
					new Communication::Models::Responses::ResponsesData::GetPwmItemsData(
						_pwmService->getPwmItems()));
			_communicationService->sendResponse(pwmItemsResponse);

			xSemaphoreGive(mutex);
		}
	}
}

void readCommunicationDataTask(void* argument)
{
	SemaphoreHandle_t mutex = (SemaphoreHandle_t)argument;
	while (true)
	{
		xSemaphoreTake(mutex, portMAX_DELAY);
		if (_communicationService->availableToRead())
		{
			Communication::Models::Requests::BaseRequest* request = _communicationService->readRequest();
			_requestService->handleRequest(request);
		}
		xSemaphoreGive(mutex);
	}
}

#endif