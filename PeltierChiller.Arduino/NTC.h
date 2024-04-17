#pragma once

#include "BaseSensor.h"

namespace Models::TemperatureSensors
{

	class NTC :
		public Models::TemperatureSensors::BaseSensor
	{
	private:

		const float _zeroCbyK = 273.15f;

		uint8_t _sensorAddress;
		uint32_t _resistance;
		uint16_t _bCoefficient; 
		uint32_t _resistanceNTC;
		uint8_t _baseNTCTemp;

	protected:


	public:

		NTC(uint8_t sensorAddress, uint32_t resistance, uint16_t bCoefficient, uint32_t resistanceNTC,
			Models::Enums::TemperatureSensorTarget sensorTarget, uint8_t baseNTCTemp = 25);
		float getTemperature();
		void sensorRequest() override;
		DynamicJsonDocument createPayload() override;
		uint8_t getSensorAddress();
	};
}