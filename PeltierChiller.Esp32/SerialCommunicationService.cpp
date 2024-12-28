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
	Serial.println(data);
}

String Communication::Services::SerialCommunicationService::readData()
{
	if (Serial.available())
	{
		return Serial.readString();
	}
	return "";
}
