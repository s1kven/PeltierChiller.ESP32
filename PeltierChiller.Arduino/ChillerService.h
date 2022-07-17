#pragma once

#include <LinkedList.h>
#include "TemperatureService.h"

namespace Services
{
	class ChillerService
	{
	private:
		Services::TemperatureService* _temperatureService;
		DeviceAddress _tSensorsAddress[];

	protected:


	public:

		ChillerService(uint8_t _temperatureSensorsPin, DeviceAddress _tSensors[], float _targetCircuitTemperature);

		void handleChillerState();
	};
}