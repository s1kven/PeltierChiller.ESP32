#pragma once
#include "Services/JsonService.h"
#include "Services/LogService.h"
#include "SerialCommunicationService.h"

Communication::Services::SerialCommunicationService::SerialCommunicationService(uint32_t baudRate) : Services::CommunicationService()
{
	_baudRate = baudRate;
}

void Communication::Services::SerialCommunicationService::init()
{
	Serial.setTimeout(0);
	Serial.setRxBufferSize(4096);
	Serial.setTxBufferSize(4096);
	Serial.begin(_baudRate);
}

void Communication::Services::SerialCommunicationService::sendData(String data)
{
	CommunicationService::sendData(data);
	Serial.println(data);
}

void Communication::Services::SerialCommunicationService::sendResponse(
	Communication::Models::Responses::Response* response)
{
	sendData(_jsonService->serializeObject(response));
	response->clear();
	delete response;
}

Communication::Models::Requests::BaseRequest* Communication::Services::SerialCommunicationService::readRequest()
{
	return Communication::Services::CommunicationService::readRequest(readData());
}

uint32_t Communication::Services::SerialCommunicationService::availableToRead()
{
	return Serial.available();
}

String Communication::Services::SerialCommunicationService::readData()
{
	if (Serial.available())
	{
		String content = Serial.readString();
		if(_logService != nullptr)
		{
			_logService->addToLogQueue(content);
		}
		return content;
	}
	return "";
}
