
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
#include "CommandService.h"
#include "CommandError.h"
#include "UpdateConfigurationCommand.h"
#include "GetTemperatureSensorsData.h"
#include "GetPwmItemsData.h"

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

	_configurationService = new Services::ConfigurationService();
	configurationError = _configurationService->readConfigurationFromSd();

	SemaphoreHandle_t communicationMutex = xSemaphoreCreateMutex();
	xTaskCreate(
		readCommunicationDataTask,
		"readCommunicationDataTask",
		4096,
		communicationMutex,
		3,
		NULL
	);

	if (*configurationError)
	{
		Communication::Models::Responses::Response* configReadResponse =
			new Communication::Models::Responses::Response(Communication::Enums::ResponseType::errorConfiguration, false,
				nullptr, configurationError->getErrorMessage());
		_communicationService->sendResponse(configReadResponse);
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
		Communication::Models::Responses::Response* temperatureSensorsResponse = 
			new Communication::Models::Responses::Response(Communication::Enums::ResponseType::temperatureSensors,
				new Communication::Models::Responses::ResponsesData::GetTemperatureSensorsData(
					_chillerService->getTemperatureService()->getTemperatureSensors()));
		_communicationService->sendResponse(temperatureSensorsResponse);

		Communication::Models::Responses::Response* pwmItemsResponse =
			new Communication::Models::Responses::Response(Communication::Enums::ResponseType::pwms,
				new Communication::Models::Responses::ResponsesData::GetPwmItemsData(
					_pwmService->getPwmItems()));
		_communicationService->sendResponse(pwmItemsResponse);

		xSemaphoreGive(mutex);
	}
}

void readCommunicationDataTask(void* argument)
{
	SemaphoreHandle_t mutex = (SemaphoreHandle_t)argument;
	while (true)
	{
		xSemaphoreTake(mutex, portMAX_DELAY);
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
				Communication::Models::Responses::Response* commandResponse =
					new Communication::Models::Responses::Response(Communication::Enums::ResponseType::errorCommand, false,
						nullptr, "Failed to parse command." + request);
				_communicationService->sendResponse(commandResponse);
			}

			Serial.println(esp_get_free_heap_size());
			Serial.println(uxTaskGetStackHighWaterMark(NULL));
		}

		request.clear();
		xSemaphoreGive(mutex);
	}
}

void initGlobalConfiguration()
{
	_communicationDelay = _configurationService->getConfiguration()->getTimersConfiguration()->getCommunicationDelay();
}

#endif