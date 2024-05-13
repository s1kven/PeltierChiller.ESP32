#pragma once

#include <Wire.h>
#include <LinkedList.h>
#include "TemperatureService.h"
#include "ChillerState.cpp"
#include "ChillerConfiguration.h"
#include "Button.h"

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
		Models::Button* _powerButton;
		uint8_t _powerButtonPin; 
		uint8_t _chillerPsSignalPin; 
		uint8_t _chillerSignalPin;
		uint8_t _powerSignalPin;

		bool _powerSignalFlag = false;
		uint32_t _powerSignalTimer = 0;
		uint32_t _oldPowerSignalTimer = 0;

		uint32_t _chillerLoadTimer = 0;
		uint16_t _varResistorValue;

		void initConfiguration();
		void manageChillerLoad();
		int computePID(float _currentT, float _targetT, float _kp, float _ki, float _kd, float _dt);
		void handleChillerState(float pcVoltage);
		void handlePowerButton();

	protected:


	public:

		ChillerService(uint8_t _temperatureSensorsPin, uint8_t _tSensorsCount, Models::TemperatureSensors::BaseSensor* _tSensors[],
			Communication::Models::ChillerConfiguration* chillerConfiguration,
			uint8_t powerButtonPin, uint8_t chillerPsSignalPin, uint8_t chillerSignalPin, uint8_t powerSignalPin);

		void manageChiller(float pcVoltage);
	};
}