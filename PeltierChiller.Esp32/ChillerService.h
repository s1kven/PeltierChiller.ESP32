#pragma once
#ifndef _ChillerService_
#define _ChillerService_ 

#include <Wire.h>
#include <LinkedList.h>
#include "TemperatureService.h"
#include "ChillerState.cpp"
#include "PinsConfiguration.h"
#include "TimersConfiguration.h"
#include "ChillerConfiguration.h"
#include "Configuration.h"
#include "Button.h"
#include "ChillerType.cpp"
#include <ESP32AnalogRead.h>

namespace Services
{
	class ChillerService
	{
	private:
		Models::Enums::ChillerType _chillerType;
		float _setTemperature;
		uint8_t _pcVoltagePin;
		ESP32AnalogRead _adc;
		float _vin = 0;
		float _voltmeterThreshold;
		uint32_t _voltmeterR1;
		uint32_t _voltmeterR2;
		float _targetTemperature;
		bool _isDelayEnablingPc;

		uint32_t _sensorsRequestDelay;
		uint32_t _buttonMinPressTime;
		uint32_t _computePidDelay;

		uint16_t _potentiometerAddress;
		uint16_t _maxPotentiometerValue;
		uint16_t _minPotentiometerValue;
		float _kp;
		float _ki;
		float _kd;
		float _dt;
		float _pidRatio;
		float _minIntegral;
		float _maxIntegral;

		uint8_t _powerButtonPin;
		uint8_t _chillerPsSignalPin;
		uint8_t _chillerSignalPin;
		uint8_t _powerSignalPin;

		Communication::Models::Configurations::Configuration* _configuration;
		Services::TemperatureService* _temperatureService;
		Models::Enums::ChillerState _state;
		Models::Button* _powerButton;

		bool _powerSignalFlag = false;
		uint32_t _powerSignalTimer = 0;
		uint32_t _oldPowerSignalTimer = 0;

		uint32_t _chillerLoadTimer = 0;
		int16_t _varResistorValue;
		uint8_t _chillerLoadPercentage = 0;

		void initConfiguration(); 
		void handlePcVoltage();
		void manageChillerLoad();
		int computePID(float _currentT, float _targetT, float _kp, float _ki, float _kd, float _dt);
		void handleChillerState(float pcVoltage);
		void handlePowerButton();
		void setPotentiometerValue(uint16_t address, uint16_t value);

	protected:


	public:

		ChillerService(Communication::Models::Configurations::Configuration* configuration);

		void manageChiller();
		Services::TemperatureService* getTemperatureService();
		uint8_t getChillerLoadPercentage();
	};
}
#endif