#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "CommunicationService.h"

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
	};
}