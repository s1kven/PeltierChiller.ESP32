#pragma once

#include <Wire.h>
#include <LinkedList.h>
#include "TemperatureService.h"
#include "ChillerState.cpp"
#include "ChillerConfiguration.h"

namespace Services
{
	class ChillerService
	{
	private:
		const uint16_t _sensorsRequestDelay = 2000;

		Communication::Models::ChillerConfiguration* _chillerConfiguration;
		Services::TemperatureService* _temperatureService;
		float _targetTemperature;
		Models::Enums::ChillerState _state;

		void initConfiguration();

	protected:


	public:

		ChillerService(uint8_t _temperatureSensorsPin, uint8_t _tSensorsCount, Models::TemperatureSensors::BaseSensor* _tSensors[],
			Communication::Models::ChillerConfiguration* chillerConfiguration, Models::Enums::ChillerState state);

		float getTargetTemperature();
		Services::TemperatureService* getTemperatureService();
		void execute();
		void handleChillerState();
		int computePID(float _currentT, float _targetT, float _kp, float _ki, float _kd, float _dt);
		Models::Enums::ChillerState GetChillerState();
		void SetChillerState(Models::Enums::ChillerState state);
	};
}