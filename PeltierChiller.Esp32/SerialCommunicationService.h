#pragma once
#ifndef _SerialCommunicationService_
#define _SerialCommunicationService_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "CommunicationService.h"

namespace Services {
	class JsonService;
}

namespace Communication
{
	namespace Services
	{
		class SerialCommunicationService :
			public CommunicationService
		{
		private:
			uint32_t _baudRate;

		public:
			SerialCommunicationService(uint32_t baudRate);

			void init() override;

			void sendData(String data) override;
			void sendResponse(Communication::Models::Responses::Response* response) override;

			String readData() override;
		};
	}
}
#endif