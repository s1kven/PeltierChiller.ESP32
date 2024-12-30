#pragma once
#include "JsonService.h"
#include "SerialCommunicationService.h"

Communication::Services::SerialCommunicationService::SerialCommunicationService(uint32_t baudRate) : Services::CommunicationService()
{
	_baudRate = baudRate;
}

void Communication::Services::SerialCommunicationService::init()
{
	Serial.setTimeout(0);
	Serial.setRxBufferSize(1024);
	Serial.begin(_baudRate);
}

void Communication::Services::SerialCommunicationService::sendData(String data)
{
	Serial.println(data);
}

void Communication::Services::SerialCommunicationService::sendResponse(
	Communication::Models::Responses::Response* response)
{
	sendData(_jsonService->serializeObject(response));
	response->clear();
	delete response;
}

String Communication::Services::SerialCommunicationService::readData()
{
	if (Serial.available())
	{
		return Serial.readString();
	}
	return "";
}
