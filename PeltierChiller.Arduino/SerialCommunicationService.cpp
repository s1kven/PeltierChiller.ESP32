#include "SerialCommunicationService.h"

Services::SerialCommunicationService::SerialCommunicationService(uint32_t baudRate) : Services::CommunicationService()
{
	_baudRate = baudRate;
}

void Services::SerialCommunicationService::init()
{
	Serial.begin(_baudRate);
}
