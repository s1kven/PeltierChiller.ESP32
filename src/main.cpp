#ifndef _ENTRYPOINT_
#define _ENTRYPOINT_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <GyverBME280.h>
#include <Wire.h>
#include <LinkedList.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Services/ChillerService.h"
#include "Models/TemperatureSensors/BaseSensor.h"
#include "Services/JsonService.h"
#include "Helpers/JsonHelper.h"
#include "Communication/Services/CommunicationService.h"
#include "Communication/Services/SerialCommunicationService.h"
#include "Services/FileService.h"
#include "Services/ConfigurationService.h"
#include "Services/PwmService.h"
#include "Services/RequestService.h"
#include "Services/WifiService.h"
#include "Services/TimeService.h"
#include "Services/LogService.h"
#include "Communication/Models/Responses/ResponsesData/GetTemperatureSensorsData.h"
#include "Communication/Models/Responses/ResponsesData/GetPwmItemsData.h"

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

Services::WifiService* _wifiService;

Services::TimeService* _timeService;

Services::LogService* _logService;

uint64_t _initMillis = 0;

struct {
	SemaphoreHandle_t _communicationMutex;
	SemaphoreHandle_t _chillerMutex;
	SemaphoreHandle_t _wifiMutex;
	SemaphoreHandle_t _logMutex;
} Mutexes;

void manageChillerTask(void* argument);
void handlePwmsTask(void* argument);
void sendCommunicationDataTask(void* argument);
void readCommunicationDataTask(void* argument);
void wifiConnectionTask(void* argument);
void logTask(void* argument);

void setup()  
{
	_temperatureService = nullptr;
	_chillerService = nullptr;
	_pwmService = nullptr;
	_wifiService = nullptr;
	_timeService = nullptr;
	_logService = nullptr;

	Wire.begin();

	_communicationService = new Communication::Services::SerialCommunicationService(460800);
	_communicationService->init();

	_requestService = new Services::RequestService();

	_fileService = new Services::FileService();
	_fileService->init(SD_CS);
	_jsonService = new Services::JsonService();

	_configurationService = new Services::ConfigurationService();
	Communication::Models::Responses::Response* configurationResponse = _configurationService->readConfigurationFromSd();

	Mutexes._communicationMutex = xSemaphoreCreateMutex();
	Mutexes._chillerMutex = xSemaphoreCreateMutex();
	Mutexes._wifiMutex = xSemaphoreCreateMutex();
	Mutexes._logMutex = xSemaphoreCreateMutex();

	xTaskCreate(
		readCommunicationDataTask,
		"readCommunicationDataTask",
		4096,
		NULL,
		3,
		NULL
	);

	if (!configurationResponse->getSuccess())
	{
		_communicationService->sendResponse(configurationResponse);
		return;
	}

	_configurationService->initConfiguration();

	xTaskCreate(
		manageChillerTask,
		"manageChillerTask",
		4096,
		NULL,
		3,
		NULL
	);

	xTaskCreate(
		handlePwmsTask,
		"handlePwmsTask",
		4096,
		NULL,
		3,
		NULL
	);

	xTaskCreate(
		sendCommunicationDataTask,
		"sendCommunicationDataTask",
		4096,
		NULL,
		3,
		NULL
	);

	xTaskCreate(
		wifiConnectionTask,
		"wifiConnectionTask",
		4096,
		NULL,
		3,
		NULL
	);

	if(_logService->getIsEnabled())
	{
		xTaskCreate(
			logTask,
			"logTask",
			4096,
			NULL,
			3,
			NULL
		);
	}
}

void loop() { }

void manageChillerTask(void* argument)
{
	while (true)
	{
		vTaskDelay(10 / portTICK_PERIOD_MS);
		if (!_configurationService->isChangeConfiguration())
		{
			xSemaphoreTake(Mutexes._chillerMutex, portMAX_DELAY);
			_chillerService->manageChiller();
			xSemaphoreGive(Mutexes._chillerMutex);
		}
	}
}

void handlePwmsTask(void* argument)
{
	while (true)
	{
		vTaskDelay(_pwmService->getUpdatePwmDelay() / portTICK_PERIOD_MS);
		if (!_configurationService->isChangeConfiguration())
		{
			xSemaphoreTake(Mutexes._chillerMutex, portMAX_DELAY);
			_pwmService->handlePwms(_chillerService->getChillerLoadPercentage());
			xSemaphoreGive(Mutexes._chillerMutex);
		}
	}
}

void sendCommunicationDataTask(void* argument)
{
	while (true)
	{
		vTaskDelay(_communicationDelay / portTICK_PERIOD_MS);
		if (!_configurationService->isChangeConfiguration())
		{
			xSemaphoreTake(Mutexes._communicationMutex, portMAX_DELAY);
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

			xSemaphoreGive(Mutexes._communicationMutex);
		}
	}
}

void readCommunicationDataTask(void* argument)
{
	while (true)
	{
		xSemaphoreTake(Mutexes._communicationMutex, portMAX_DELAY);
		xSemaphoreTake(Mutexes._chillerMutex, portMAX_DELAY);
		xSemaphoreTake(Mutexes._wifiMutex, portMAX_DELAY);
		xSemaphoreTake(Mutexes._logMutex, portMAX_DELAY);
		if (_communicationService->availableToRead())
		{
			Communication::Models::Requests::BaseRequest* request = _communicationService->readRequest();
			_requestService->handleRequest(request);
		}
		xSemaphoreGive(Mutexes._logMutex);
		xSemaphoreGive(Mutexes._wifiMutex);
		xSemaphoreGive(Mutexes._chillerMutex);
		xSemaphoreGive(Mutexes._communicationMutex);
	}
}

void wifiConnectionTask(void* argument)
{
	while (true)
	{
		xSemaphoreTake(Mutexes._wifiMutex, portMAX_DELAY);
		_wifiService->tryConnect();
		xSemaphoreGive(Mutexes._wifiMutex);
		if(millis() > _wifiService->getReconnectionTimeout() + _initMillis)
		{
			vTaskDelay(_wifiService->getReconnectionTimeout() / portTICK_PERIOD_MS);
		}
		else
		{
			vTaskDelay(50 / portTICK_PERIOD_MS);
		}
	}
}

void logTask(void* argument)
{
	while (true)
	{
		xSemaphoreTake(Mutexes._logMutex, portMAX_DELAY);
		_logService->pushContentToLog();
		xSemaphoreGive(Mutexes._logMutex);
		vTaskDelay(_logService->getLogDelay() / portTICK_PERIOD_MS);
	}
}

#endif