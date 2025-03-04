#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

namespace Communication::Services
{
	class CommunicationService
	{
	public:

		virtual void init() = 0;

		virtual void sendData(String data) = 0;
		virtual String readData() = 0;
	};
}