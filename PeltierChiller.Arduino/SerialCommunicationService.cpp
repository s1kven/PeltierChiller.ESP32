#include "SerialCommunicationService.h"

Communication::Services::SerialCommunicationService::SerialCommunicationService(uint32_t baudRate) : Services::CommunicationService()
{
	_baudRate = baudRate;
}

void Communication::Services::SerialCommunicationService::init()
{
	Serial.begin(_baudRate);
}

void Communication::Services::SerialCommunicationService::sendData(String data)
{
	Serial.println(data);
}

String Communication::Services::SerialCommunicationService::readData()
{
	return Serial.readString();
}
