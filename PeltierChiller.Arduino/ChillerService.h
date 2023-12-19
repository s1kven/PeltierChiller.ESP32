#pragma once

#include <Wire.h>
#include <LinkedList.h>
#include "TemperatureService.h"
#include "ChillerState.cpp"

namespace Services
{
	class ChillerService
	{
	private:
		Services::TemperatureService* _temperatureService;
		float _targetTemperature;
		Models::Enums::ChillerState _state;

	protected:


	public:

		ChillerService(uint8_t _temperatureSensorsPin, uint8_t _tSensorsCount, Models::TemperatureSensors::BaseSensor* _tSensors[], float _targetCircuitTemperature, Models::Enums::ChillerState state);

		float getTargetTemperature();
		Services::TemperatureService* getTemperatureService();
		void handleChillerState();
		int computePID(float _currentT, float _targetT, float _kp, float _ki, float _kd, float _dt);
		Models::Enums::ChillerState GetChillerState();
		void SetChillerState(Models::Enums::ChillerState state);
	};
}