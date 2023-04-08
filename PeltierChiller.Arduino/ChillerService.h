#pragma once

#include <Wire.h>
#include <LinkedList.h>
#include "TemperatureService.h"

namespace Services
{
	class ChillerService
	{
	private:
		Services::TemperatureService* _temperatureService;
		float _targetTemperature;

	protected:


	public:

		ChillerService(uint8_t _temperatureSensorsPin, Models::TemperatureSensor* _tSensors[], float _targetCircuitTemperature);

		float getTargetTemperature();
		void handleChillerState();
		int computePID(float _currentT, float _targetT, float _kp, float _ki, float _kd, float _dt);
	};
}